type AnyFunction = (...args: any[]) => any;

// Measure function execution time
export const measure = async <T>(
  func: AnyFunction,
  ...args: Parameters<typeof func>
) => {
  // Start time
  const start = getCurrentTime();

  // Check type of function
  if (func.constructor.name === "AsyncFunction") {
    // Async function execution
    const result = (await func(...args)) as T;
    // End time
    const end = getCurrentTime();
    return { result, duration: Number(end - start) };
  } else {
    // Sync function execution
    const result = func(...args) as T;
    // End time
    const end = getCurrentTime();
    return { result, duration: Number(end - start) };
  }
};

// Get current time in nanoseconds
function getCurrentTime() {
  return process.hrtime.bigint();
}
