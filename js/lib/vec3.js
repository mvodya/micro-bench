// Three dimension vector
export default class Vec3 {
  constructor(x = 0, y = 0, z = 0) {
    this.x = x;
    this.y = y;
    this.z = z;
  }

  // Summation
  add(v) {
    return new Vec3(this.x + v.x, this.y + v.y, this.z + v.z);
  }

  // Subtraction
  subtract(v) {
    return new Vec3(this.x - v.x, this.y - v.y, this.z - v.z);
  }

  // Multiplication by scalar
  multiply(v) {
    return new Vec3(this.x * v, this.y * v, this.z * v);
  }

  // Dot product
  dot(v) {
    return this.x * v.x + this.y * v.y + this.z * v.z;
  }

  // Vector length
  length() {
    return Math.sqrt(this.x ** 2 + this.y ** 2 + this.z ** 2);
  }

  // Get vector normalize
  normalize() {
    const len = this.length();
    return len > 0 ? this.multiply(1 / len) : new Vec3(0, 0, 0);
  }

  // Print vector info
  toString() {
    return `Vec3(${this.x}, ${this.y}, ${this.z})[${this.length()}]`;
  }

  // Check vector equals
  equals(v) {
    return this.x == v.x && this.y == v.y && this.z == v.z;
  }
}
