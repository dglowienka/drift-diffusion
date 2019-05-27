#include "../include/drift_diffusion.h"
#include <numeric>

double Drift_diffusion::B(double z)
{
	double tmp_B;
	tmp_B = z / (exp(z) - 1);

	return tmp_B;
}

// Regular gaussian elimination algorithm
vector<double> Drift_diffusion::gaussian_elimination(vector<vector<double> > A_matrix, vector<double> b_data)
{
	int n = b_data.size();
	auto tmp_b = b_data.begin(); //better hope they have same elements as numbers
	for (auto &row : A_matrix)
	{
		row.push_back(*tmp_b++); //right to left so this works
	}

	for (int i = 0; i<n; i++) {
		// Search for maximum in this column
		double maxEl = abs(A_matrix[i][i]);
		int maxRow = i;
		for (int k = i + 1; k<n; k++) {
			if (abs(A_matrix[k][i]) > maxEl) {
				maxEl = abs(A_matrix[k][i]);
				maxRow = k;
			}
		}

		// Swap maximum row with current row (column by column)
		for (int k = i; k<n + 1; k++) {
			double tmp = A_matrix[maxRow][k];
			A_matrix[maxRow][k] = A_matrix[i][k];
			A_matrix[i][k] = tmp;
		}

		// Make all rows below this one 0 in current column
		for (int k = i + 1; k<n; k++) {
			double c = -A_matrix[k][i] / A_matrix[i][i];
			for (int j = i; j<n + 1; j++) {
				if (i == j) {
					A_matrix[k][j] = 0;
				}
				else {
					A_matrix[k][j] += c * A_matrix[i][j];
				}
			}
		}
	}

	// Solve equation Ax=b for an upper triangular matrix A
	vector<double> x(n);
	for (int i = n - 1; i >= 0; i--) {
		x[i] = A_matrix[i][n] / A_matrix[i][i];
		for (int k = i - 1; k >= 0; k--) {
			A_matrix[k][n] -= A_matrix[k][i] * x[i];
		}
	}
	return x;
}

// Thomson algorithm for tridiagonal matrix Ax=b but A=LU with Lower and Upper part of triangle
// [beta, 	gamma,	0,		0	 ] [x0] = [b1]
// [alpha,	beta,	gamma,	0	 ] [x1] = [b2]
// [0,		alpha,	beta,	gamma] [x2] = [b3]
// [0,		0,		alpha,	beta ] [x3] = [b4]
vector<double> Drift_diffusion::thomson_algorithm(vector<double> &alpha, vector<double> &beta, vector<double> &gamma, vector<double> &b, int n)
{
	vector<double> x;
	x.resize(n); // Need for back substitution

	// Decomposition and forward substitution
	for (int i = 0; i < n-1; ++i)
	{
		alpha[i] /= beta[i];
		beta[i+1] -= alpha[i]*gamma[i];
		b[i+1] -= alpha[i]*b[i];
	}

	// Back substitution
	x[n-1] = b[n-1]/beta[n-1];
	for (int i = n-1; i-- > 0;) 
		x[i] = (b[i]-gamma[i]*x[i+1])/beta[i];

	return x;
}

double Drift_diffusion::average_vector(vector<double> vector_in)
{
	double tmp_average;

	// Calculate average of the vector
	tmp_average = accumulate(vector_in.begin(), vector_in.end(), 0.0) / vector_in.size();

	return tmp_average;
}

// Returns interpolated value at x from parallel arrays ( xData, yData )
//   Assumes that xData has at least two elements, is sorted and is strictly monotonic increasing
//   boolean argument extrapolate determines behaviour beyond ends of array (if needed)
double Drift_diffusion::interpolate(vector<double> &xData, vector<double> &yData, double x, bool extrapolate)
{
   int size = xData.size();

   int i = 0;                                                                  // find left end of interval for interpolation
   if ( x >= xData[size - 2] )                                                 // special case: beyond right end
   {
      i = size - 2;
   }
   else
   {
      while ( x > xData[i+1] ) i++;
   }
   double xL = xData[i], yL = yData[i], xR = xData[i+1], yR = yData[i+1];      // points on either side (unless beyond ends)
   if ( !extrapolate )                                                         // if beyond ends of array and not extrapolating
   {
      if ( x < xL ) yR = yL;
      if ( x > xR ) yL = yR;
   }

   double dydx = ( yR - yL ) / ( xR - xL );                                    // gradient

   return yL + dydx * ( x - xL );                                              // linear interpolation
}