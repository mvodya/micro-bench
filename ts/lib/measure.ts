type AnyFunction = (...args: any[]) => any;

// Measure function execution time
export const measure = async <T>(
  func: (...args: any[]) => T,
  ...args: Parameters<typeof func>
) => {
  // Start time
  const start = getCurrentTime();

  // Async or sync function execution
  const result = await Promise.resolve(func(...args));

  // End time
  const end = getCurrentTime();
  return { result, duration: Number(end - start) };
};

// Get current time in nanoseconds
function getCurrentTime() {
  return process.hrtime.bigint();
}
