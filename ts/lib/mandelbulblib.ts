import Vec3 from "./vec3";

// Maximum number of steps for ray marching
export const MAX_MARCHING_STEPS = 100;
// Maximum marching distance before stopping
export const MAX_DISTANCE = 100.0;
// Minimum distance threshold to consider on surface
export const SURFACE_DISTANCE = 0.001;

// Maximum iterations for distance estimator function
export const MAX_ITERATIONS = 8;
// Escape radius for distance estimator
export const BAILOUT = 2.0;
// Power exponent for Mandelbulb fractal shape
export const POWER = 8.0;

// Distance estimator (DE) function for the Mandelbulb fractal
// Calculate minimum distance from point to fractal
export function mandelbulbDE(pos: Vec3) {
  // Position
  let z = pos;
  // Distance increment
  let dr = 1.0;
  // Distance
  let r = 0.0;

  for (let i = 0; i < MAX_ITERATIONS; i++) {
    r = z.length();
    if (r > BAILOUT) break;

    // Convert to spherical coordinates
    const theta = Math.acos(z.z / r);
    const phi = Math.atan2(z.y, z.x);
    const zr = Math.pow(r, POWER);

    // Scale the distance increment
    dr = Math.pow(r, POWER - 1.0) * POWER * dr + 1.0;

    // Calculate new coordinates
    const sinTheta = Math.sin(POWER * theta);
    const cosTheta = Math.cos(POWER * theta);
    const sinPhi = Math.sin(POWER * phi);
    const cosPhi = Math.cos(POWER * phi);

    z = new Vec3(
      zr * sinTheta * cosPhi,
      zr * sinTheta * sinPhi,
      zr * cosTheta,
    ).add(pos);
  }
  return (0.5 * Math.log(r) * r) / dr;
}

// Calculate normal vector at a surface point
export function getNormal(pos: Vec3) {
  const eps = 0.001;
  const dx =
    mandelbulbDE(pos.add(new Vec3(eps, 0, 0))) -
    mandelbulbDE(pos.subtract(new Vec3(eps, 0, 0)));
  const dy =
    mandelbulbDE(pos.add(new Vec3(0, eps, 0))) -
    mandelbulbDE(pos.subtract(new Vec3(0, eps, 0)));
  const dz =
    mandelbulbDE(pos.add(new Vec3(0, 0, eps))) -
    mandelbulbDE(pos.subtract(new Vec3(0, 0, eps)));
  const normal = new Vec3(dx, dy, dz);
  return normal.normalize();
}

// Ray marching function to find distance to the surface
export function rayMarch(from: Vec3, dir: Vec3) {
  let distance = 0.0;
  for (let i = 0; i < MAX_MARCHING_STEPS; i++) {
    const pos = from.add(dir.multiply(distance));
    const d = mandelbulbDE(pos);
    if (d < SURFACE_DISTANCE) {
      // Intersection
      return distance;
    }
    if (distance > MAX_DISTANCE) {
      // No intersection
      break;
    }
    distance += d;
  }
  // No intersection found
  return -1.0;
}
