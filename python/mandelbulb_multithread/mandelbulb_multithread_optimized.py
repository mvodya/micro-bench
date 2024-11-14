from PIL import Image
import numpy as np
import time
from numba import njit, prange, get_num_threads

WIDTH = 2048
HEIGHT = 2048

# Maximum number of steps for ray marching
MAX_MARCHING_STEPS = 100
# Maximum marching distance before stopping
MAX_DISTANCE = 100.0
# Minimum distance threshold to consider on surface
SURFACE_DISTANCE = 0.001

# Maximum iterations for distance estimator function
MAX_ITERATIONS = 8
# Escape radius for distance estimator
BAILOUT = 2.0
# Power exponent for Mandelbulb fractal shape
POWER = 8.0

# Distance estimator (DE) function for the Mandelbulb fractal
# Calculate minimum distance from point to fractal
@njit
def mandelbulbDE(pos):
  # Position
  z = pos.copy()
  # Distance increment
  dr = 1.0
  # Distance
  r = 0.0

  for i in range(MAX_ITERATIONS):
    r = np.linalg.norm(z)
    if r > BAILOUT:
      break

    # Convert to spherical coordinates
    theta = np.arccos(z[2] / r)
    phi = np.arctan2(z[1], z[0])
    zr = r ** POWER

    # Scale the distance increment
    dr = r ** (POWER - 1.0) * POWER * dr + 1.0

    # Calculate new coordinates
    sinTheta = np.sin(POWER * theta)
    cosTheta = np.cos(POWER * theta)
    sinPhi = np.sin(POWER * phi)
    cosPhi = np.cos(POWER * phi)

    z = np.array([zr * sinTheta * cosPhi, zr * sinTheta * sinPhi, zr * cosTheta]) + pos

  return 0.5 * np.log(r) * r / dr

# Calculate the normal vector at a surface point
@njit
def getNormal(pos):
  eps = 0.001
  dx = mandelbulbDE(pos + np.array([eps, 0, 0])) - mandelbulbDE(pos - np.array([eps, 0, 0]))
  dy = mandelbulbDE(pos + np.array([0, eps, 0])) - mandelbulbDE(pos - np.array([0, eps, 0]))
  dz = mandelbulbDE(pos + np.array([0, 0, eps])) - mandelbulbDE(pos - np.array([0, 0, eps]))
  normal = np.array([dx, dy, dz])
  return normal / np.linalg.norm(normal) # normalize

# Ray marching function to find distance to the surface
@njit
def rayMarch(f, dir):
  distance = 0.0
  for i in range(MAX_MARCHING_STEPS):
    pos = f + dir * distance
    d = mandelbulbDE(pos)
    if d < SURFACE_DISTANCE:
      # Intersection
      return distance
    if distance > MAX_DISTANCE:
      # No intersection
      break
    distance += d
  # No intersection found
  return -1.0

@njit(parallel=True)
def mandelbulb():
  image = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)

  cameraPos = np.array([0.0, 0.0, -1.6])
  lightDir = np.array([1.0, 1.0, -1.0])
  lightDir /= np.linalg.norm(lightDir) # normalize

  for y in prange(HEIGHT):
    for x in range(WIDTH):
      # Normalize pixel coordinates
      px = (x / WIDTH) * 2.0 - 1.0
      py = (y / HEIGHT) * 2.0 - 1.0
      # Adjust for aspect ratio
      px *= WIDTH / HEIGHT

      # Calculate ray direction
      dir = np.array([px, py, 1.0])
      dir /= np.linalg.norm(dir) # normalize

      dist = rayMarch(cameraPos, dir)
      if dist > 0.0:
        # Calculate intersection point and normal
        pos = cameraPos + dir * dist
        normal = getNormal(pos)

        # Compute lighting (Lambertian shading)
        diffuse = max(0.0, np.dot(normal, lightDir))

        # Set pixel color based on lighting
        color = int(diffuse * 255)
        image[y, x] = [color, color, color]
      else:
        # Set background color
        image[y, x] = [0, 0, 0]

  return image

STEPS = 10

print("[PYTHON OPTIMIZED] Mandelbulb multi threaded benchmark")
print(f"Number of available threads: {get_num_threads()}\nSize: {WIDTH} x {HEIGHT}\nPOWER: {POWER} / MAX ITERATIONS: {MAX_MARCHING_STEPS}")

for i in range(STEPS):
  print(f"Start rendering {i + 1}/{STEPS}")

  start = time.time_ns()
  data = mandelbulb()
  duration = time.time_ns() - start

  print(f"[{i + 1}] Execution time: {duration} ns ({duration / 1e9} s)")

  if i == 0:
    print("Save first run to file mandelbulb.png")
    image = Image.fromarray(data, 'RGB')
    image.save('mandelbulb.png')

print("All renderings are done")