#ifndef TTLCONDTRIGDEBUG_H_DEFINED
#define TTLCONDTRIGDEBUG_H_DEFINED

// Diagnostic tattle macros.


// Enable/disable switch.
#define TRIGWANTDEBUG 0

// Input override switch. This generates a test pattern on the inputs.
#define TRIGDEBUG_WANTINPUTPATTERN 0


// Reporting ID stride for input banks, for readability.
#define TRIGDEBUG_INPUT_ID_STRIDE 10
// Reporting ID offset for output banks, for readability.
#define TRIGDEBUG_OUTPUT_ID_OFFSET 100


// Report prefix string. This defaults to "[CondTrig]  ".
#ifndef TRIGDEBUGPREFIX
#define TRIGDEBUGPREFIX "[CondTrig]  "
#endif

// Conditional code block.
#if TRIGWANTDEBUG
#define T_DEBUG(x) do { x } while(false);
#else
#define T_DEBUG(x) {}
#endif

// Debug tattle output.
// Flushing should already happen with std::endl, but force it anyways.
#define T_PRINT(x) T_DEBUG(std::cout << TRIGDEBUGPREFIX << x << std::endl << std::flush;)


#endif
