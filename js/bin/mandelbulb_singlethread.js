#!/usr/bin/env node

import { measure, Vec3, mandelbulblib } from "../lib/index.js";
import fs from "fs";
import { createCanvas } from "canvas";

// Image width
const WIDTH = 2048;
// Image height
const HEIGHT = 2048;

// Number of renders to be performed (identical renders for quality check)
const STEPS = 10;

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
      const dist = mandelbulblib.rayMarch(cameraPos, dir);
      const index = (x + y * WIDTH) * 4;
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

  // Save image data and return it
  ctx.putImageData(imageData, 0, 0);
  return image;
}

//////

(async () => {
  console.log("[JS] Mandelbulb single thread benchmark");
  console.log(
    `Size: ${WIDTH} x ${HEIGHT}\nPOWER: ${mandelbulblib.POWER} / MAX ITERATIONS: ${mandelbulblib.MAX_MARCHING_STEPS}`,
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
