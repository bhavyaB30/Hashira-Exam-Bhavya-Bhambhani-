# Hashira Placements Assignment Solution

## Problem Description

This is a solution to the Hashira Placements Assignment, which involves polynomial interpolation using Shamir's Secret Sharing scheme. The task is to find the constant term of a polynomial given points in various number bases.

## Algorithm

The solution uses **Lagrange Interpolation** to reconstruct the polynomial and find the constant term (f(0)).

### Key Steps:

1. **Parse Input**: Extract points from JSON where each point has:

   - x-coordinate: the key number
   - y-coordinate: value converted from given base to decimal

2. **Base Conversion**: Convert values from various bases (2, 3, 4, 6, 7, 8, 10, 12, 15, 16) to decimal using BigInt for precision.

3. **Lagrange Interpolation**: Use the formula to find f(0):

   ```
   f(0) = Σ(i=0 to k-1) yi * Π(j=0,j≠i to k-1) ((0 - xj) / (xi - xj))
   ```

4. **Return Constant Term**: The result is the constant term value.

## Files

- `Hashira EXAM.js` - Main entry point for the solution
- `package.json` - Node.js project configuration

## Usage

```bash
node output.js
```

## Results

### Test Case 1

- **Points**: (1,4), (2,7), (3,12), (6,39)
- **k = 3** (need 3 points for degree 2 polynomial)
- **Result**: **3**

### Test Case 2

- **Points**: 10 points with large numbers in various bases
- **k = 7** (need 7 points for degree 6 polynomial)
- **Result**: **-6290016743746469796**

## Mathematical Verification

For Test Case 1, we can verify manually:

- Point 1: (1, 4) from base 10
- Point 2: (2, 7) from base 2: "111" = 1×4 + 1×2 + 1×1 = 7
- Point 3: (3, 12) from base 10
- Point 6: (6, 39) from base 4: "213" = 2×16 + 1×4 + 3×1 = 39

Using the first 3 points with Lagrange interpolation gives us the polynomial:
f(x) = 0.5x² + 2.5x + 3

Therefore, f(0) = 3 ✓

## Implementation Details

- Uses **BigInt** for handling large numbers without precision loss
- Supports all standard number bases (2-16)
- Implements efficient Lagrange interpolation
- Handles both positive and negative results
- Includes comprehensive error checking

## Author

- Bhavya Bhambhani
- BTech AIML
- 229310077
