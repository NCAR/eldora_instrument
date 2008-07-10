#include "BuiltinFilters.h"

//////////////////////////////////////////////////////////////////
BuiltinFilters::BuiltinFilters() {
	
}

//////////////////////////////////////////////////////////////////

void BuiltinFilters::init(double key, unsigned int coeffs[]) {

	(*this)[key].resize(sizeof(coeffs));

	for (int i = 0; i < (*this)[key].size(); i++) {
		(*this)[key][i] = coeffs[i];
	}
}

