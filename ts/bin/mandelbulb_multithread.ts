#!/usr/bin/env node

import { measure, Vec3, mandelbulblib } from "../lib/index";
import fs from "fs";
import { createCanvas, Canvas } from "canvas";
import { Worker } from "worker_threads";
import os from "os";


// Image width
const WIDTH = 2048;
// Image height
const HEIGHT = 2048;

// Number of renders to be performed (identical renders for quality check)
const STEPS = 10;

// Number of workers
const THREAD_COUNT = os.cpus().length;

// Worker location
const WORKER = new URL("./mandelbulb_worker_kernel", import.meta.url);

async function mandelbulb() {
  return new Promise<Canvas>((resolve) => {
    const cameraPos = new Vec3(0.0, 0.0, -1.6);
    const lightDir = new Vec3(1.0, 1.0, -1.0).normalize();

    const workers = [];
    const segmentHeight = Math.ceil(HEIGHT / THREAD_COUNT);
    let completedWorkers = 0;
    const image = createCanvas(WIDTH, HEIGHT);
    const ctx = image.getContext("2d");
    const imageData = ctx.createImageData(WIDTH, HEIGHT);

    // Run threads
    for (let i = 0; i < THREAD_COUNT; i++) {
      const startY = i * segmentHeight;
      const endY = Math.min(startY + segmentHeight, HEIGHT);
      // Start worker
      const worker = new Worker(WORKER, {
        workerData: {
          startY,
          endY,
          width: WIDTH,
          height: HEIGHT,
          cameraPosData: cameraPos,
          lightDirData: lightDir,
        },
      });

      // Wait work worker complete renderer
      worker.on("message", ({ startY, buffer }) => {
        // Merge data from completed threads
        imageData.data.set(buffer, startY * WIDTH * 4);
        completedWorkers++;
        if (completedWorkers === THREAD_COUNT) {
          ctx.putImageData(imageData, 0, 0);
          resolve(image);
        }
      });

      workers.push(worker);
    }
  });
}

//////

// MAIN THREAD
(async () => {
  console.log("[JS] Mandelbulb multi threaded benchmark");
  console.log(`Number of available threads: ${THREAD_COUNT}`);
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