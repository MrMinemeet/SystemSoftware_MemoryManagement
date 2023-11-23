#include "Heap.hpp" // Include the corresponding header file
#include "Block.hpp"


// Define the member functions of the Heap class here
Heap::Heap() {
    this->heap_buffer = std::malloc(HEAP_SIZE);

#if DEBUG
	// Fill memory with 0xff (for debugging)
	for(int i = 0; i < HEAP_SIZE; ++i) {
		((char*)heap_buffer)[i] = (char)0xff;
	}
#endif

	this->free = {new (heap_buffer) Block(HEAP_SIZE - sizeof(Block))}; // - sizeof(Block) because of the overhead
	this->free_bytes = HEAP_SIZE;


    std::cout << "A new heap has been created!" << std::endl;
}

Heap::~Heap() {
    std::free(heap_buffer);
    std::cout << "A heap has been destroyed!" << std::endl;
}

Block* Heap::alloc(int size) {
	std::cout << "Trying to allocate " << size << " bytes of memory..." << std::endl;
	if (size > free_bytes) {
		std::cout << "Not enough memory!" << std::endl;
		return nullptr;
	}
    // Implement the allocation logic here

	// Look for free block that fits the to be allocated size + bit of overhead (sizeof(Block))
	Block* current = nullptr;
	for (Block* block: free) {
		current = block;
		if (current->size >= size+sizeof(Block)) {
			break;
		}
	}

	// Check if last block in "free" list is big enough
	if (current->size < size+sizeof(Block)) {
		std::cout << "Not enough memory!" << std::endl;
		return nullptr;
	}

	Block* p = current;
	int new_size = current->size - size - sizeof(Block);
	if (new_size >= 2*sizeof(Block)) { // split block
		char* atBack = (char*) current + current->size + sizeof(Block);
		char* withSpaceForBlock = atBack - size;
		char* withOverhead = withSpaceForBlock;
		char* position = withOverhead;
		//char* position = (char*) current + current->size - size - sizeof(Block);
		p = new (position) Block(size);
		current->size = new_size;
	} else { // remove block from freelist
		free.remove(current);
	}
	p->used = true;

#if DEBUG
	// Fill memory with 0xab (for debugging)
	for(int i = sizeof(Block); i < size; ++i) {
		((char*)p)[i] = (char)0xab;
	}
#endif

	free_bytes -= size + sizeof(Block);

	std::cout << "Allocated " << size << " bytes of memory at " << p << std::endl;
	return p;
}

void Heap::dealloc() {
    std::cout << "Deallocating memory..." << std::endl;
    // Implement the reallocation logic here
}

/// Register a type with the Heap. Will return true if the type was successfully registered, false otherwise.
bool Heap::registerType(std::string& type, TypeDescriptor& descriptor) {
	if (type_map.find(type) != type_map.end()) {
		std::cout << "Type " << type << " already exists!" << std::endl;
		return false;
	}
	std::cout << "Registering type " << type << std::endl;
	type_map[type] = &descriptor;
	return true;
}

std::string Heap::ToString() {
	std::string postfix;
	std::string str = "Heap size: " + std::to_string(HEAP_SIZE) + "\n";
	str += "Free bytes: " + std::to_string(free_bytes) + "\n";


	str += "Free blocks {\n";
	for (Block* block : free) {
		str += "\t" + block->ToString() + postfix + "\n";
		postfix = ", ";
	}
	str += "}\n";

	return str;
}
