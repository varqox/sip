#include "../sip_package.hh"

#include <simlib/macros/stack_unwinding.hh>

namespace commands {

void genout() {
    STACK_UNWINDING_MARK;

    SipPackage sp;
    sp.generate_test_output_files();

    if (access("Simfile", F_OK) == 0) {
        sp.save_limits();
    }
    sp.warn_about_tests_not_specified_as_static_or_generated();
}

} // namespace commands
