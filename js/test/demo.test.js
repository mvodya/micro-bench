import { expect } from 'chai';
import { demo } from '../lib/index.js';

describe('demo', () => {
    it('should return "demo" string', () => {
        expect(demo.demoStr()).to.equal("DEMO");
    });
});
