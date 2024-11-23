#!/usr/bin/env node

import { measure } from "../lib/index.js";

// Image width
const WIDTH = 64;
// Image height
const HEIGHT = 64;

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

function mandelbulb() {
}

console.log("[JS] Mandelbulb single thread benchmark");
console.log(`Size: ${WIDTH} x ${HEIGHT}\nPOWER: ${POWER} / MAX ITERATIONS: ${MAX_MARCHING_STEPS}\n`);

// Steps
for (let i = 0; i < STEPS; i++) {
  console.log(`Start rendering ${i + 1}/${STEPS}`);
  const {image, duration} = await measure(mandelbulb);
  console.log(`[${i + 1}] Execution time: ${duration} ns (${Number(duration) / 1e9} s)`);

  if (i === 0) {
    console.log('Save first run to file mandelbulb.png');
  }
}
