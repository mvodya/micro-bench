#!/usr/bin/env node

import { measure } from "../lib/index.js";
import { Vec3 } from "../lib/index.js";
import fs from "fs";
import { createCanvas } from "canvas";

// Image width
const WIDTH = 2048;
// Image height
const HEIGHT = 2048;

// Maximum number of steps for ray marching
const MAX_MARCHING_STEPS = 100;
// Maximum marching distance before stopping
const MAX_DISTANCE = 100.0;
// Minimum distance threshold to consider on surface
const SURFACE_DISTANCE = 0.001;

// Maximum iterations for distance estimator function
const MAX_ITERATIONS = 8;
// Escape radius for distance estimator
const BAILOUT = 2.0;
// Power exponent for Mandelbulb fractal shape
const POWER = 8.0;

// Number of renders to be performed (identical renders for quality check)
const STEPS = 10;

// Distance estimator (DE) function for the Mandelbulb fractal
// Calculate minimum distance from point to fractal
function mandelbulbDE(pos) {
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
function getNormal(pos) {
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
function rayMarch(from, dir) {
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

function mandelbulb() {
  const image = createCanvas(WIDTH, HEIGHT);
  const ctx = image.getContext("2d");
  const imageData = ctx.createImageData(WIDTH, HEIGHT);
  const buffer = imageData.data;

  const cameraPos = new Vec3(0.0, 0.0, -1.6);
  const lightDir = new Vec3(1.0, 1.0, -1.0).normalize();

  for (let y = 0; y < HEIGHT; y++) {
    for (let x = 0; x < WIDTH; x++) {
      // Normalize pixel coordinates
      const px = (x / WIDTH) * 2.0 - 1.0;
      const py = (y / HEIGHT) * 2.0 - 1.0;
      // Adjust px for aspect ratio
      const aspectRatio = WIDTH / HEIGHT;
      const adjustedPx = px * aspectRatio;

      // Calculate ray direction
      const dir = new Vec3(adjustedPx, py, 1.0).normalize();
      const dist = rayMarch(cameraPos, dir);
      const index = (x + y * WIDTH) * 4;
      if (dist > 0.0) {
        // Calculate intersection point and normal
        const pos = cameraPos.add(dir.multiply(dist));
        const normal = getNormal(pos);

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

  // Save image data and return it
  ctx.putImageData(imageData, 0, 0);
  return image;
}

//////

(async () => {
  console.log("[JS] Mandelbulb single thread benchmark");
  console.log(
    `Size: ${WIDTH} x ${HEIGHT}\nPOWER: ${POWER} / MAX ITERATIONS: ${MAX_MARCHING_STEPS}`,
  );

  // Steps
  for (let i = 0; i < STEPS; i++) {
    console.log(`Start rendering ${i + 1}/${STEPS}`);
    const { result: image, duration } = await measure(mandelbulb);
    console.log(
      `[${i + 1}] Execution time: ${duration} ns (${Number(duration) / 1e9} s)`,
    );

    if (i === 0) {
      console.log("Save first run to file mandelbulb.png");
      const out = fs.createWriteStream("mandelbulb.png");
      const stream = image.createPNGStream();
      stream.pipe(out);
      await new Promise((resolve) => out.on("finish", resolve));
    }
  }
})();
