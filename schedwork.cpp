#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper (const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, std::vector<Worker_T>& shifts, size_t workingRow, size_t workingCol);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    for (size_t r = 0; r < avail.size(); r++) {
        sched.push_back(std::vector<Worker_T>(dailyNeed, INVALID_ID));
    }
    std::vector<Worker_T> shifts(avail[0].size(), 0); //to count number of shifts per worker

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0, 0);


}

bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, std::vector<Worker_T>& shifts, size_t workingRow, size_t workingCol) {
    
    if (workingRow >= avail.size() && workingCol >= dailyNeed) { //if workingRow >= number of days && workingCol >= daily need we've hit our final case
        return true;
    }

    size_t nextRow;
    size_t nextCol;

    if (workingCol == dailyNeed-1 && workingRow == sched.size()-1) { //if we are at the very last row and column in our schedule set the next place to search to our base case
        nextRow = workingRow + 1;
        nextCol = workingCol + 1;
    } else { //otherwise we still are working through
        if (workingCol + 1 >= dailyNeed) {
            nextCol = 0;
            nextRow = workingRow + 1;
        } else {
            nextCol = workingCol + 1;
            nextRow = workingRow;
        }
    }
    
    for (size_t i = 0; i < avail[workingRow].size(); i++) { //move through every available worker for todar
        if (avail[workingRow][i] != 0 && (std::find(sched[workingRow].begin(), sched[workingRow].end(), i)  == sched[workingRow].end()) ) { //if worker is available && isn't already working 
            //std::cout << "Slot " << workingRow << " " << workingCol << " says: Worker " << i << " isn't busy on day " << workingRow << std::endl;
            sched[workingRow][workingCol] = i; //set schedule at current position to that worker
            shifts[i] += 1; //add to their shifts
            if (shifts[i] <= maxShifts) {
                if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, nextRow, nextCol)) {
                    return true;
                }
            }
            shifts[i] -= 1;
            sched[workingRow][workingCol] = INVALID_ID;
        }
    }
    return false;

}

