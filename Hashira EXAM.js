
class HashiraSolver {
  convertToDecimal(value, base) {
    let result = 0n;
    const bigBase = BigInt(base);

    for (const digit of value) {
      let digitValue;
      if (digit >= "0" && digit <= "9") {
        digitValue = BigInt(digit.charCodeAt(0) - "0".charCodeAt(0));
      } else {
        digitValue = BigInt(
          digit.toLowerCase().charCodeAt(0) - "a".charCodeAt(0) + 10
        );
      }
      result = result * bigBase + digitValue;
    }
    return result;
  }

  parsePoints(data) {
    const points = [];
    Object.keys(data).forEach((key) => {
      if (key !== "keys" && data[key].base && data[key].value) {
        const x = BigInt(key);
        const y = this.convertToDecimal(
          data[key].value,
          parseInt(data[key].base)
        );
        points.push({ x, y });
      }
    });
    return points.sort((a, b) => (a.x < b.x ? -1 : a.x > b.x ? 1 : 0));
  }

  lagrangeInterpolation(points, k) {
    const selectedPoints = points.slice(0, k);
    let result = 0n;

    for (let i = 0; i < selectedPoints.length; i++) {
      let numerator = selectedPoints[i].y;
      let denominator = 1n;

      for (let j = 0; j < selectedPoints.length; j++) {
        if (i !== j) {
          numerator *= 0n - selectedPoints[j].x;
          denominator *= selectedPoints[i].x - selectedPoints[j].x;
        }
      }
      result += numerator / denominator;
    }
    return result;
  }

  solve(jsonInput) {
    const data = JSON.parse(jsonInput);
    const points = this.parsePoints(data);
    return this.lagrangeInterpolation(points, data.keys.k);
  }
}

// Test cases
const testCase1 = {
  keys: { n: 4, k: 3 },
  1: { base: "10", value: "4" },
  2: { base: "2", value: "111" },
  3: { base: "10", value: "12" },
  6: { base: "4", value: "213" },
};

const testCase2 = {
  keys: { n: 10, k: 7 },
  1: { base: "6", value: "13444211440455345511" },
  2: { base: "15", value: "aed7015a346d635" },
  3: { base: "15", value: "6aeeb69631c227c" },
  4: { base: "16", value: "e1b5e05623d881f" },
  5: { base: "8", value: "316034514573652620673" },
  6: { base: "3", value: "2122212201122002221120200210011020220200" },
  7: { base: "3", value: "20120221122211000100210021102001201112121" },
  8: { base: "6", value: "20220554335330240002224253" },
  9: { base: "12", value: "45153788322a1255483" },
  10: { base: "7", value: "1101613130313526312514143" },
};

const solver = new HashiraSolver();

console.log("HASHIRA PLACEMENTS ASSIGNMENT RESULTS");
console.log(
  "Test Case 1 Answer:",
  solver.solve(JSON.stringify(testCase1)).toString()
);
console.log(
  "Test Case 2 Answer:",
  solver.solve(JSON.stringify(testCase2)).toString()
);
console.log("");
console.log("Solution completed successfully!");
