#pragma once

//Header file for all the Events

//Forward Declarations
struct Event;
struct WaitUntil;
struct ParallelRace;
struct ParallelDeadline;
struct ParallelEvent;
struct SequentialEvent;
struct ConditionalEvent;
struct RepeatEvent;
struct FunctionalEvent;
struct InstantEvent;
struct RunEvent;

//Include files
#include "Event.h"
#include "ConditionalEvent.h"
#include "ParallelDeadline.h"
#include "ParallelEvent.h"
#include "ParallelRace.h"
#include "SequentialEvent.h"
#include "RepeatEvent.h"
#include "WaitUntil.h"
#include "InstantEvent.h"
#include "RunEvent.h"