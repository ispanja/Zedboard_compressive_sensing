/*
	The code below is a test case for a compressive sensing reconstruction algorithm applied to a single image from the MNIST dataset.
	Eigen template library is used for easier matrix handling. URL: http://eigen.tuxfamily.org/index.php?title=Main_Page
								... for some examples see: eigen_testing.cpp
	Written by: Karlo Sever
*/

//#include "images_training_set.h"		// MNIST dataset
//#include "labels_training_set.h"

#include <iostream>
#include "test_img.h"					// Digit: 0
#include "eig_vect_matrix.h"			// Eigenvectors taken from the '0' dictionary
#include "measurement_matrix.h"

#include <Eigen/Dense>

using namespace Eigen;

#define DIM 784


//Test image of digit '0' is of dimensions X_test[784,1] stored in X_test_img[784]
//Measurement matrix is of dimensions phi[78,784] stored in phi_vect[78*784]
//Measured signal y = phi*X_test is of dimensions [78,1]

int main(void) {
	VectorXd X_test(DIM);
	for (int i = 0; i < DIM; i++)
		X_test(i) = X_test_img[i];

	MatrixXd phi(78, DIM);
	int indx = 0;
	for (int i = 0; i < 78; i++) {
		for (int j = 0; j < DIM; j++) {
			phi(i, j) = phi_vect[indx++];
			printf("%.6f", phi(i, j));
		}
		printf("\n");
	}

	MatrixXd v0(DIM, DIM);
	indx = 0;
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			v0(i, j) = eigv[indx++];
		}
	}

	for (int i = 0; i < DIM; i++) {
		printf("%6.2f", X_test[i]);
		if (i % 28 == 0)
			printf("\n");
	}
	std::cout << "\nMeasured signal y... \n" << phi * X_test << std::endl;

	//MatrixXd A(DIM, DIM), B(DIM, 78);
	//A = v0.transpose()*phi.transpose()*phi*v0;
	//B = v0.transpose()*phi.transpose();
	//alpha_rec = A.inverse()*B*(phi*X_test);

	VectorXd alpha_rec(DIM), X_rec(DIM), recon(DIM);
	X_rec = phi.transpose()*(phi*X_test);
	alpha_rec = v0.transpose()*X_rec;					// Reconstruction coefficients


	recon = v0 * alpha_rec;								// Reconstructed data
	std::cout << "\n\nReconstructed data...\n" << std::endl;
	for (int i = 0; i < DIM; i++) {
		printf("%6.2f ", recon[i]);
		if (i % 28 == 0) {
			printf("\n");
		}
	}

	return 0;
}
