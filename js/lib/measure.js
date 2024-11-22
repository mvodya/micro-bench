// Measure function execution time
export const measure = async (func, ...args) => {
  // Start time
  const start = getCurrentTime();

  // Check type of function
  if (func.constructor.name === "AsyncFunction") {
    // Async function execution
    const result = await func(...args);
    // End time
    const end = getCurrentTime();
    return { result, duration: Number(end - start) };
  } else {
    // Sync function execution
    const result = func(...args);
    // End time
    const end = getCurrentTime();
    return { result, duration: Number(end - start) };
  }
}

// Get current time in nanoseconds
function getCurrentTime() {
  return process.hrtime.bigint();
}