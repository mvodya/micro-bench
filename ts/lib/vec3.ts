// Three dimensional vector
export default class Vec3 {
  x: number;
  y: number;
  z: number;

  constructor(x: number = 0, y: number = 0, z: number = 0) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  // Summation
  add(v: Vec3): Vec3 {
    return new Vec3(this.x + v.x, this.y + v.y, this.z + v.z);
  }

  // Subtraction
  subtract(v: Vec3): Vec3 {
    return new Vec3(this.x - v.x, this.y - v.y, this.z - v.z);
  }

  // Multiplication by scalar
  multiply(v: number): Vec3 {
    return new Vec3(this.x * v, this.y * v, this.z * v);
  }

  // Dot product
  dot(v: Vec3): number {
    return this.x * v.x + this.y * v.y + this.z * v.z;
  }

  // Vector length
  length(): number {
    return Math.sqrt(this.x ** 2 + this.y ** 2 + this.z ** 2);
  }

  // Get vector normalize
  normalize(): Vec3 {
    const len = this.length();
    return len > 0 ? this.multiply(1 / len) : new Vec3(0, 0, 0);
  }

  // Print vector info
  toString(): string {
    return `Vec3(${this.x}, ${this.y}, ${this.z})[${this.length()}]`;
  }

  // Check vector equals
  equals(v: Vec3): boolean {
    return this.x === v.x && this.y === v.y && this.z === v.z;
  }
}
