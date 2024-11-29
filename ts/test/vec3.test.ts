import { expect } from "chai";
import { Vec3 } from "../lib/index";

describe("Vec3 Class", () => {
  // Test for creating a vector with default values
  it("should create a vector with default values", () => {
    const vec = new Vec3();
    expect(vec.x).to.equal(0);
    expect(vec.y).to.equal(0);
    expect(vec.z).to.equal(0);
  });

  // Test for creating a vector with specified values
  it("should create a vector with specified values", () => {
    const vec = new Vec3(1, 2, 3);
    expect(vec.x).to.equal(1);
    expect(vec.y).to.equal(2);
    expect(vec.z).to.equal(3);
  });

  // Test for adding two vectors
  it("should add two vectors correctly", () => {
    const v1 = new Vec3(1, 2, 3);
    const v2 = new Vec3(4, 5, 6);
    const result = v1.add(v2);
    expect(result.equals(new Vec3(5, 7, 9))).to.be.true;
  });

  // Test for subtracting two vectors
  it("should subtract two vectors correctly", () => {
    const v1 = new Vec3(4, 5, 6);
    const v2 = new Vec3(1, 2, 3);
    const result = v1.subtract(v2);
    expect(result.equals(new Vec3(3, 3, 3))).to.be.true;
  });

  // Test for multiplying a vector by a scalar
  it("should multiply a vector by a scalar correctly", () => {
    const vec = new Vec3(1, 2, 3);
    const result = vec.multiply(2);
    expect(result.equals(new Vec3(2, 4, 6))).to.be.true;
  });

  // Test for calculating dot product
  it("should calculate dot product correctly", () => {
    const v1 = new Vec3(1, 2, 3);
    const v2 = new Vec3(4, 5, 6);
    const dotProduct = v1.dot(v2);
    expect(dotProduct).to.equal(32);
  });

  // Test for calculating vector length
  it("should calculate vector length correctly", () => {
    const vec = new Vec3(3, 4, 0);
    expect(vec.length()).to.equal(5);
  });

  // Test for normalizing a vector
  it("should normalize a vector correctly", () => {
    const vec = new Vec3(3, 0, 0);
    const normalized = vec.normalize();
    expect(normalized.equals(new Vec3(1, 0, 0))).to.be.true;
  });

  // Test for normalizing a zero-length vector
  it("should return a zero vector when normalizing a zero-length vector", () => {
    const vec = new Vec3(0, 0, 0);
    const normalized = vec.normalize();
    expect(normalized.equals(new Vec3(0, 0, 0))).to.be.true;
  });

  // Test for string representation of a vector
  it("should return a correct string representation of the vector", () => {
    const vec = new Vec3(1, 0, 1);
    expect(vec.toString()).to.include(`Vec3(1, 0, 1)[${Math.sqrt(2)}]`);
  });

  // Test for checking equality of two vectors
  it("should correctly check equality of two vectors", () => {
    const v1 = new Vec3(1, 2, 3);
    const v2 = new Vec3(1, 2, 3);
    const v3 = new Vec3(4, 5, 6);
    expect(v1.equals(v2)).to.be.true;
    expect(v1.equals(v3)).to.be.false;
  });
});
