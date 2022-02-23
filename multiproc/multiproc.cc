#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "parent: " << ::getpid() << std::endl;
    switch(::fork()) {
	case 0:
	    std::cout << "child pid: " << ::getpid() << std::endl;
	    std::cout << "child's parent pid: " << ::getppid() << std::endl;

	switch(::fork()) {
	    case 0:
	        std::cout << "grandchild pid: " << ::getpid() << std::endl;
	        std::cout << "grandchild's parent pid: " << ::getppid() << std::endl;
	        std::cout << "grandchild quitting" << std::endl;
		return 0;
            case -1:
	    	std::cout << "grandchild failure" << std::endl;
	    default:
	    	std::cout << "child spawned" << std::endl;
	    }
	std::cout << "child quitting" << std::endl;
	return 0;
	
	case -1:
	    std::cout << "child failure" << std::endl;
	    return -1;

	default:
	    std::cout << "child spawned" << std::endl;
    }
    std::cout << "parent quitting" << std::endl;
    return 0;
}
