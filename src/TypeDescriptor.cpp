#include "TypeDescriptor.hpp"

// Define member functions for the TypeDescriptor class here, if needed
TypeDescriptor::TypeDescriptor(int obj_size, int pointer_offset_array[], int offset_amount) {
    // Constructor implementation
    this->totalSize = obj_size;
	this->pointerOffsetArray = pointer_offset_array;
	this->offsetAmount = offset_amount;
}

TypeDescriptor::~TypeDescriptor() {
    // Destructor implementation
    delete(this->pointerOffsetArray);
}

std::string TypeDescriptor::ToString() {
	// ToString implementation
	std::string str = "TypeDescriptor { ";
	str += "Object dataSize: " + std::to_string(this->totalSize) + ", ";
	str += "Pointer offset array: [";
	for (int i = 0; i < this->offsetAmount; i++) {
		str += std::to_string(this->pointerOffsetArray[i]);
		if (i != this->offsetAmount - 1) {
			str += ", ";
		}
	}
	str += "], ";
	str += "Offset amount: " + std::to_string(this->offsetAmount);
	str += " }";
	return str;
}