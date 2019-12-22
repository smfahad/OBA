// ///
// OBATargetMachine.h
//

#ifndef OBATargetMachine_H 
#define OBATargetMachine_H 


// WARNING : 
// In your application, unless your C++ Development 
// Environment provides you means to finely tune the order 
// of initialization of global static objects (i.e. on a 
// compilation unit basis, what Visual C++ 6.0 doesn't), 
// if an object initialization depends on static data 
// members of the class OBATargetMachine below, you should 
// not create this object as global (creating that way can 
// lead to hard to diagnose wrong behaviour, because those 
// data may be initialized before the values they depend 
// on).
// Since there is a broad chance to do so in a normal OBA 
// application, there is only one safe way: 
// Avoid unnecessary global static objects. Whenever an 
// object absolutely needs global scope (in a file or 
// externally among files), declare a global static pointer 
// instead, then create the needed object through the 
// pointer by dynamic allocation in the main or later on, 
// then use reference  declarations to rename the resulting 
// created objects everywhere.


// REMARK :
// Reading anything that shows in the private parts here
// is a user loss of time (as usual). Moreover, the 
// private parts here simulate mechanisms normally 
// implemented by hardware, that are not soft changing.




// --------- Debut modif linux ---------
// Les types et classes suivants étaient initialement définis
// dans la classe OBATargetMachine. Ils en ont été extraits 
// afin de clarifier leur accessibilité, car cette classe ne peut
// pas être considérée comme un espace de nommage, ce qui proscrit
// toute commande de type "using OBATargetMachine::..."

// The target microcontroller is able to manage interrupts.
// An interrupt is nothing more than a register offset in a 
// register set, called the interrupt vector, that has special 
// execution semantics. Each register in the interrupt vector
// is called an interrupt slot. The microcontroller is able to 
// manage as many interrupts as there are slots in the 
// interrupt vector.
// The BASIC SPECIAL SEMANTICS of each slot are the following:
//  When an interrupt occurs, the microcontroller senses some
//  data bus lines that are interpreted as the offset of a 
//  desired slot (this offset is the interrupt identity). 
//  The slot contents are interpreted as the address of an 
//  interrupt handler, i.e. an executable routine that expects 
//  one parameter. The microcontroller then suspends normal 
//  program execution, and calls the addressed interrupt 
//  handler, passing it its own slot offset as parameter. This 
//  call is performed in a context that is different from 
//  normal program execution. Whenever the interrupt handler 
//  returns, the normal program execution resumes exactly 
//  where it was suspended (an interrupt handler should always 
//  return).
// Interrupt handlers are also called interrupt service 
// routines, or ISRs). 
// 
// Use KEY: you will find an "interruptVector" declared below.
//  After writing your own interrupt handlers , assign 
//  each relevant slot one of them. 
// Example:
//    void myHandler (Interrupt);
//    OBATargetMachine::interruptVector[IT_timer] = myHandler;
// 
// ADDITIONAL SEMANTICS:
// Initially, the address in each slot points to code the 
//  execution of which stalls program execution (no return). 
//  In order to prevent interrupts from hitting the program 
//  before safe interrupt handlers are set, all interrupts 
//  are initially disabled. Any hit is then ignored.
//  The user shall eventually enable desired interrupts 
//  before getting for them the basic semantics described 
//  above. The interrupt enable mask should be used to do 
//  that, as explained below. 
//  When a bit is set in the interrupt mask, the matching 
//  interrupt is enabled. 
//  When the bit is cleared, this interrupt  is disabled.
// There is also an auto-reset mask:
//  When a bit is set in the auto-reset mask, after normal 
//  performance on the first next matching interrupt hit, 
//  the corresponding handler is removed and its slot is 
//  turned back to stalling code address. 
//  When that bit is cleared, the same interrupt handler 
//  remains in the slot after performance. 
//  Initially, for each interrupt the handler is stable 
//  (cleared bit).


typedef unsigned Interrupt; 
// i.e. offset of the interrupt vector slot that hardware 
// is wired to operate with when sensed on data bus upon  
// an interrupt signal.
// Remark: symbolic names for OBA relevant interrupts are
// provided at the end of this file, in the BSD part.


typedef void (* InterruptHandler) (Interrupt);


class InterruptVectorSlot {
  // An Interrupt Vector Slot is a very special hardware
  // register, it has special assignment semantics. You 
  // cannot create one by yourself (it is not a software 
  // entity, but the software representation of a 
  // hardware entity), you can just load a value in it, 
  // or transfer there the contents of another Slot, or 
  // also reset it.
  friend class OBATargetMachine;
  friend class InterruptMaskRegister;
  friend class InterruptExecution;

public:
  void operator= (InterruptHandler); 
  void operator= (const InterruptVectorSlot&); 
  // ^ overriding implicit one.
  void reset();
  // ^ after resetting, an occurrence of the slot
  // matched interrupt will stall the program, unless 
  // the matching bit had been cleared in the enable 
  // mask register.

private:
  InterruptHandler interruptHandler;
  bool enabled;
  bool autoReset;
  bool connected;
  InterruptVectorSlot(); // you cannot add hardware!
}; // InterruptVectorSlot
// Short hand for convenience:
typedef InterruptVectorSlot ItVSlot; 


// Interrupt masks:
typedef unsigned InterruptMask; 
// They should be handled using bitwise logical 
// operators (like '~' '&' '|' or '^') or shifting. 
// They work after having being assigned to some 
// mask register.


class InterruptMaskRegister {
  // An Interrupt Mask Register is a very special 
  // hardware register, it has special assignment 
  // semantics too:
  friend class OBATargetMachine;

public:
  void operator= (InterruptMask itm);
  operator unsigned () const; 
  // ^ intended: operator InterruptMask () const;

private:
  InterruptVectorSlot * const mate;
  bool ItVSlot::* impactedSlotMemberOffset;
  InterruptMaskRegister // you cannot add hardware!
   (bool ItVSlot::* impactedSlotMemberOffset,
	InterruptVectorSlot * const mate);
}; // InterruptMaskRegister
// Short hand for convenience:
typedef InterruptMaskRegister ItMReg; 


// OBA specific BSP (board support package). 
// The two enum types below provide symbolic names 
// respectively identifying  interrupts and related
// interrupt masks that are relevant to the user of the 
// OBA Target Machine (which is bound to some specific
// devices).
// The symbolic name of each indicate to OBA developpers
// which is the related environment device.
// Use KEY:
//|   interruptVector [(OTM::Interrupt&)(IT_timer)] = someHandler;
// works, but the explicit cast is not neede, coercion works too:
//|   interruptVector [IT_timer] = someHandler;
// end of use key.
enum InterruptId 
{
	IT_timer            =  5,
	IT_drivingControls  = 14,
	IT_controlPanel     = 15
}; 
enum InterruptMaskId
{
	atTimerInterrupt            = 0x0020, // 2**5
	atDrivingControlsInterrupt  = 0x4000, // 2**14
	atControlPanelInterrupt     = 0x8000, // 2**15
	allObaRelatedInterrupts     = atTimerInterrupt
		                          | atDrivingControlsInterrupt
			                      | atControlPanelInterrupt
};
// The imposed values shown here are relevant to the
// compiler only, not to OBA developpers. 
// Using enum types (as opposite to introducing symbolic 
// names as constants of another type) allow to show them 
// in a header file. 
// They are given for information only, and they are meant 
// to derive from interface design decisions (after the 
// IRS, namely in the IDD) that have been kept outside the 
// scope of this case study, and thus have been directly 
// integrated to the BSP of present OBA Target Machine 
// specification. 
// Remark: strictly speaking, the description of 
// functionally named ports here above also derive from 
// IDD, and should belong to the BSP.


// Solid stuff itself:
class OBATargetMachine { 

	// Think of it as a microcontroller.

public:
   // Provision for communicating with sensors 
   // and actuators in the physical environment through ports 
   // by reading from and writing to related buffers 
   // (refer to Safe Drive IRS for detailed layout of each 
   // pointed buffer below) :
   // cruising automobile to OBA:
   static void * pulseCounterWordAddress;
   static void * throttlePositionWordAddress;
   // driver to OBA:
   static void * drivingControlsInterruptWordAddress;
   static void * drivingControlsStateWordAddress;
   static void * controlPanelInterruptWordAddress;
   // OBA to cruising automobile:
   static void * throttleCommandWordAddress;
   // OBA to driver:
   static void * displayBufferAddress;
   static void * ledCommandByteAddress;


   // Interrupts
   // The target machine interrupt vector, array of 
   // slots governing the basic semantics of interrupts:
   #define lastSlotOffset    15
   #define interruptCardinal lastSlotOffset+1
   static InterruptVectorSlot interruptVector [interruptCardinal];

   // Interrupt masks:
   // universal mask values, through constants, as easy 
   // starter for bitwise operations (but see also the 
   // BSD part at the end of this file for OBA relevant 
   // mask values):
   static const InterruptMask allInterrupts;
   static const InterruptMask noInterrupt; 
   // ^ the initial value for both masks

   // The 2 interrupt mask registers, governing the additional 
   // semantics of interrupts:
   static InterruptMaskRegister enabledInterruptsRegister; 
   static InterruptMaskRegister autoResetInterruptsRegister;
  
   // Provision for being periodically sent a specific 
   // timer interrupt by THE builtin timer in the target 
   // machine :
   static void setPeriodicTimer (int periodInMillisecond, int latencyInMillisecond);
   // After a "latencyInMillisecond" delay, the target 
   // processor on which the calling program is running 
   // receives one IT_timer interrupt every 
   // "periodInMillisecond".
   // A zero "periodInMillisecond" value immediately stops the 
   // timer (regardless of the "latencyInMillisecond" value).

   // Students have to consider the OBA Target Machine as a 
   // bare microcontroller. They have to develop applicative 
   // software without thread support from any real-time 
   // monitor (multithreading is actually not a need for 
   // achieving a most efficient OBA design).
   // However, in the special case of multithreading considered
   // an educational goal, students that want to explore a 
   // multi-threaded design may ever (and can only) rely on 
   // such support taken directly from host OS API, or from 
   // language runtime library (if support available there), 
   // and lend that the status of a real-time monitor in their 
   // design. The alternate idea of writing their own real-time 
   // scheduler is unrealistic and deprecated, since 
   // microcontroller modelling here has not been led up to 
   // emulation of registers, and doesn't allow for programming 
   // of context switching.
   // Nevertheless, the following primitive is introduced for 
   // convenience, as a useful tuning facility, since hardware 
   // threads related to interrupt handlers are implemented as 
   // host machine software threads for actual execution on the 
   // host machine.

   static void idleWait (int delayInMillisecond);
   // Suspends the calling thread as long a time as specified by
   // the parameter, leaving free the CPU resource for other 
   // threads. 

private:
   OBATargetMachine() {;} 
   // Deter instantiation compulsions, since OBATargetMachine
   // is an all-static class, so, conceptually, a single object 
   // by itself. Don't use the same pattern to create a second 
   // single object depending on this one in the design of the 
   // applicative part (see Warning at top).

   // Use KEY:
   // To get access to this single OTM object, just:
   //|#include "OBATargetMachine.h"
   // and then, call static member functions using class scope 
   // operator, e.g.:
   //|   OTM::setPeriodicTimer (200, 10);
   // end of example.

   static InterruptExecution* notATrojanHorseRepellant, & norMe;

}; // OBATargetMachine
// Short hand, for convenience:
typedef OBATargetMachine    OTM;
// ---------- Fin modif linux ----------

#endif
