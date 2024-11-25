import { Vec3, mandelbulblib } from "../lib/index.js";
import { parentPort, workerData } from "worker_threads";

// MANDELBULB MULTITHREAD WORKER
// Get worker data
const { startY, endY, width, height, cameraPosData, lightDirData } = workerData;
// Restore Vec3
const cameraPos = new Vec3(cameraPosData.x, cameraPosData.y, cameraPosData.z);
const lightDir = new Vec3(lightDirData.x, lightDirData.y, lightDirData.z);
// Vertical strip (segment) for renderer
const buffer = new Uint8ClampedArray((endY - startY) * width * 4);

for (let y = startY; y < endY; y++) {
  for (let x = 0; x < width; x++) {
    // Normalize pixel coordinates
    const px = (x / width) * 2.0 - 1.0;
    const py = (y / height) * 2.0 - 1.0;
    // Adjust px for aspect ratio
    const aspectRatio = width / height;
    const adjustedPx = px * aspectRatio;

    // Calculate ray direction
    const dir = new Vec3(adjustedPx, py, 1.0).normalize();
    const dist = mandelbulblib.rayMarch(cameraPos, dir);
    const index = ((y - startY) * width + x) * 4;
    if (dist > 0.0) {
      // Calculate intersection point and normal
      const pos = cameraPos.add(dir.multiply(dist));
      const normal = mandelbulblib.getNormal(pos);

      // Compute lighting (Lambertian shading)
      const diffuse = Math.max(0.0, normal.dot(lightDir));

      // Set pixel color based on lighting
      const color = Math.floor(diffuse * 255);
      buffer[index] = color;
      buffer[index + 1] = color;
      buffer[index + 2] = color;
      buffer[index + 3] = 255;
    } else {
      // Set background color
      buffer[index] = 0;
      buffer[index + 1] = 0;
      buffer[index + 2] = 0;
      buffer[index + 3] = 255;
    }
  }
}
// Send rendered data to main thread
parentPort.postMessage({ startY, buffer });