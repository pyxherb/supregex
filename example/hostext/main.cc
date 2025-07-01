#include <iocape/io_service.h>

int main() {
	iocape::IOServiceUniquePtr ioService(iocape::createIOService(peff::getDefaultAlloc(), peff::getDefaultAlloc()));

	if (!ioService) {
		puts("Error: Error creating IO service object");
		return ENOMEM;
	}

	return 0;
}
