#include "internal.h"
#include <perl++/perl++.h>

#ifndef GvCV_set
#define GvCV_set(gv, cv) (GvCV(gv) = cv)
#endif

namespace perl {
	Glob::Glob(interpreter* _interp, GV* _handle) : interp(_interp), handle(_handle) {
	}
	Glob::Glob(const Glob& other) : interp(other.interp), handle(other.handle) {
		SvREFCNT_inc(reinterpret_cast<SV*>(handle));
	}
	Glob::~Glob() {
		SvREFCNT_dec(reinterpret_cast<SV*>(handle));
	}

	Glob& Glob::operator=(const Scalar::Base& other) {
		if (GvSV(handle) != NULL) {
			SvREFCNT_dec(GvSV(handle));
		}
		GvSV(handle) = SvREFCNT_inc(other.get_SV(false));
		return *this;
	}
	Glob& Glob::operator=(const Array::Value& other) {
		if (GvAV(handle) != NULL) {
			SvREFCNT_dec(reinterpret_cast<SV*>(GvAV(handle)));
		}
		SvREFCNT_inc(reinterpret_cast<SV*>(other.handle));
		GvAV(handle) = other.handle;
		return *this;
	}
	Glob& Glob::operator=(const Hash::Value& other) {
		if (GvHV(handle) != NULL) {
			SvREFCNT_dec(reinterpret_cast<SV*>(GvHV(handle)));
		}
		SvREFCNT_inc(reinterpret_cast<SV*>(other.handle));
		GvHV(handle) = other.handle;
		return *this;
	}
	Glob& Glob::operator=(const Code::Value& other) {
		if (GvCV(handle) != NULL) {
			SvREFCNT_dec(reinterpret_cast<SV*>(GvCV(handle)));
		}
		SvREFCNT_inc(reinterpret_cast<SV*>(other.handle));
		GvCV_set(handle, other.handle);
		return *this;
	}
	Raw_string Glob::name() const {
		return Raw_string(GvNAME(handle), GvNAMELEN(handle), true);
	}
	Scalar::Temp Glob::scalar_value() const {
		return Scalar::Temp(interp, GvSVn(handle), false);
	}
	Array::Temp Glob::array_value() const {
		return Array::Temp(interp, GvAVn(handle), false);
	}
	Hash::Temp Glob::hash_value() const {
		return Hash::Temp(interp, GvHVn(handle), false);
	}
	Code::Value Glob::code_value() const {
		// Handle Nullcv
		return Code::Value(interp, GvCVu(handle));
	}
	const Ref<Glob>::Temp Glob::take_ref() const {
		return Ref<Glob>::Temp(interp, newRV_inc(reinterpret_cast<SV*>(handle)), true);
	}
}
