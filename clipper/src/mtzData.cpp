

#include "mtzData.h"

mtzData::mtzData() {

	clipper::String title;
	clipper::String ipfile = "NONE";
	clipper::Resolution reso;
	clipper::Grid_sampling grid;

	grid = clipper::Grid_sampling( 23, 24, 25 );

	// make data objects
	clipper::CCP4MTZfile mtzin;
	// clipper::MTZcrystal cxtl;
	// clipper::HKL_info hkls;

}

mtzData::~mtzData() {
	
}
