// ///
//  PortsLayout.h
//


// THIS FILE MUST BE IRS COMPLIANT.
// Maybe system dependent.
// The same C++ writing doesn't always lead exactly to the same
// layout, depending on the compiler. The compiler itself usually
// complies with the conventions of the OS/machine it is targeted
// to. In order for the layout to be compliant with the IRS of the
// Safe Drive subsystem, maybe this file needs a somewhat 
// different version for each compiler.

// EDUCATIONAL PITFALL !!!

// Here is the MS-Windows version, checked against MS Visual C++
// documentation, and tested using a reinterpret cast to unsigned 
// integer and printing it in hexadecimal
// .
// The following excerpt was found in: 
//  MSDN Library - January 2000>Visual Studio 6.0 Documentation>
//  Visual C++ Documentation>Reference>
//  C/C++ Language and C++ Libraries>C++ Language Reference>
//  Classes>C++ Bit Fields.
// "Microsoft Specific 
//  The ordering of data declared as bit fields is from low to
//  high bit"
// 
// High bit and low bit obviously refer to interpreting the 
// underlying bit pattern as an integral value. And 
// conventionally for integral values, the high bit is thought
// as written left, according to the direction for reading 
// usual decimal value writings. And the low bit is thought as 
// written after the high one, that is, right, or below.
// With such conventions, we can say that under Windows, with
// Visual C++, the order of writing bit fields in the source 
// text and the conventional order of writing them in memory
// are REVERSE each other.
// [The endianity issue (big endian/little endian) is still 
// a different topic, not relevant here, and concerning the 
// ordering of bytes in memory as compared to the ordering of
// bits]



#ifndef PortsLayout_H
#define PortsLayout_H

// Definition of message types that environment expects from OBA
// or sends to it.
// This document is the C++ translation of information held by 
// the IRS document. 
// It has been designed to belong to the common sources of all the
// processes in OBA Virtual Target Supplies (Cruise and Driver).
// But those processes need also the description of all other 
// messages they exchange each other. 
// However, the present stuff has been kept apart with a definite
// EDUCATIONAL purpose: 
// The students being developing OBA need to write quite similar
// stuff by reading and analyzing the IRS. Whenever the teacher
// wants to speed up this phase of the development, he or she can
// provide this source file as a very efficient helper.

struct ThrottleCommandWord {
   unsigned outputvoltagevalue  : 7;
   unsigned paritybit   : 1; // Should make odd the full byte (7+1)
} ;

struct ThrottleResponseWord { // Same layout as ThrottleCommandWord 
   unsigned inputvoltagevalue  : 7;     // but explicitly repeated to provide 
   unsigned paritybit   : 1;     // for possible future divarications.
} ;

struct CounterWord { 
   unsigned pulsecount    : 16; 
   // 
};

struct DrivingStationInterruptStatusWord {
	unsigned acceleratorFlag  : 1; 
	unsigned brakeFlag        : 1;
	unsigned clutchFlag       : 1;
	unsigned ignitionFlag     : 1;
};

struct DrivinStationStateWord {
	unsigned acceleratorState : 1; 
	unsigned brakeState       : 1;
	unsigned clutchState      : 1;
	unsigned ignitionState    : 1;
	unsigned engagedGear      : 3;
};

struct ControlPanelInterruptStatusWord {
   unsigned keyCode  : 5 ;
};

#define ControlPanelDisplaySize 20

struct DisplayBufferAddress { 
   char displaymessage[ControlPanelDisplaySize];   
};

struct LedCommandWord { 
	unsigned led_3 : 1;
   	unsigned led_2 : 1;
	unsigned led_1 : 1; // leftmost significant bit: 
                       // leftmost on control panel.
};

#endif

// end of PortsLayout.h


