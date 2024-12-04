import { expect } from "chai";
import { measure } from "../lib/index";

describe("measure Function", () => {
  // Test for synchronous function
  it("should measure execution time of a synchronous function", async () => {
    const syncFunc = (x: number, y: number) => x + y;
    const { result, duration } = await measure(syncFunc, 3, 7);

    expect(result).to.equal(10);
    expect(duration).to.be.a("number");
    expect(duration).to.be.greaterThan(0);
  });

  // Test for asynchronous function
  it("should measure execution time of an asynchronous function", async () => {
    const asyncFunc = async (x: number, y: number): Promise<number> => {
      return new Promise((resolve) => setTimeout(() => resolve(x * y), 100));
    };

    const { result, duration } = await measure(asyncFunc, 3, 7);

    expect(result).to.equal(21);
    expect(duration).to.be.a("number");
    expect(duration).to.be.greaterThanOrEqual(100_000_000); // 100 ms in nanoseconds
  });

  // Test for a function with no return value
  it("should measure execution time of a function with no return value", async () => {
    const voidFunc = () => {
      for (let i = 0; i < 1000000; i++) {} // Simple loop to consume time
    };

    const { result, duration } = await measure(voidFunc);

    expect(result).to.be.undefined; // Function doesn't return a value
    expect(duration).to.be.a("number");
    expect(duration).to.be.greaterThan(0);
  });

  // Test for a function with no arguments
  it("should measure execution time of a function with no arguments", async () => {
    const noArgFunc = () => 42;

    const { result, duration } = await measure(noArgFunc);

    expect(result).to.equal(42);
    expect(duration).to.be.a("number");
    expect(duration).to.be.greaterThan(0);
  });

  // Test for an empty function
  it("should measure execution time of an empty function", async () => {
    const emptyFunc = () => {};

    const { result, duration } = await measure(emptyFunc);

    expect(result).to.be.undefined;
    expect(duration).to.be.a("number");
    expect(duration).to.be.greaterThanOrEqual(0);
  });
});
