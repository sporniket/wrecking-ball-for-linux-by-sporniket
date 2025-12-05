# Manual Playtest Checklist: GetTheKey Levels

**Test Date:** _____________
**Tester:** _____________
**Build Version:** _____________

## Purpose

GetTheKey levels require collecting ALL key bricks to activate exit bricks, then destroying one exit brick to complete. This checklist verifies the key-and-exit mechanic works correctly.

---

## Pre-Test Setup

- [ ] Build is compiled and runs without errors
- [ ] Game launches successfully
- [ ] Can navigate to GetTheKey level
- [ ] Test GetTheKey level(s) are available

---

## Test 1: Key and Exit Identification

### Setup
- Start GetTheKey level

### Test Steps
- [ ] Key bricks clearly visible and distinct (color/icon)
- [ ] Exit bricks clearly visible and distinct (different from keys)
- [ ] Exit bricks initially appear inactive/locked (visual indicator)
- [ ] HUD shows "Keys: X/Y" counter
- [ ] Level type clearly marked as "Get the Key"
- [ ] Regular bricks (if any) visually normal

### Expected Outcome
✅ Keys and exits easily distinguishable
✅ Inactive exits visually different from active exits
✅ Player understands the two-phase mechanic

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 2: Key Collection Phase

### Setup
- GetTheKey level with 3 keys and 1 exit

### Test Steps
- [ ] Can destroy key bricks by hitting with ball
- [ ] Key counter updates when key destroyed (0/3 → 1/3 → 2/3 → 3/3)
- [ ] Exit brick remains inactive while keys remain
- [ ] Can hit inactive exit brick (nothing happens)
- [ ] Regular bricks can be destroyed (optional)
- [ ] Level does NOT complete when all keys collected

### Expected Outcome
✅ Key collection phase works correctly
✅ Exit remains inactive until all keys collected
✅ Progress clearly displayed

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 3: Exit Activation

### Setup
- GetTheKey level, collect all keys

### Test Steps
- [ ] Exit brick activates when last key collected
- [ ] Exit brick visual changes (color/glow/unlock animation)
- [ ] HUD updates to show "Exit unlocked!" or similar
- [ ] Exit brick now destructible
- [ ] Can destroy active exit brick
- [ ] Level completes when active exit destroyed

### Expected Outcome
✅ Exit activation triggers correctly
✅ Visual feedback clear and immediate
✅ Player understands they can now destroy exit

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 4: Multiple Keys Progression

### Setup
- GetTheKey level with 5 keys

### Test Steps
- [ ] Progress counter shows 0/5 initially
- [ ] Each key destruction updates counter (1/5, 2/5, 3/5, 4/5)
- [ ] Exit remains inactive through first 4 keys
- [ ] Exit activates only when 5th key destroyed
- [ ] Can destroy keys in any order
- [ ] No keys missed or double-counted

### Expected Outcome
✅ Multi-key collection tracked accurately
✅ Exit activation only on final key

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 5: Multiple Exits

### Setup
- GetTheKey level with 2 keys and 3 exits

### Test Steps
- [ ] All exit bricks initially inactive
- [ ] All exits activate when both keys collected
- [ ] Can destroy any one exit to complete level
- [ ] Level completes after destroying first exit
- [ ] Other exits don't need to be destroyed
- [ ] Completion screen shows keys collected and exit reached

### Expected Outcome
✅ Multiple exits all activate together
✅ Only one exit needs to be destroyed
✅ Player can choose which exit to use

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 6: Cannot Complete Without Keys

### Setup
- GetTheKey level, attempt to skip key collection

### Test Steps
- [ ] Try to destroy inactive exit brick (should do nothing or not register)
- [ ] Level does not complete from hitting inactive exit
- [ ] Must collect all keys first
- [ ] Visual/audio feedback indicates exit is locked
- [ ] No way to bypass key collection requirement

### Expected Outcome
✅ Exit completely ineffective when inactive
✅ Key collection is mandatory
✅ Clear feedback about lock status

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 7: Single Key, Single Exit (Simplest)

### Setup
- GetTheKey level with 1 key and 1 exit

### Test Steps
- [ ] Level displays correctly
- [ ] Progress shows "Keys: 0/1"
- [ ] Exit inactive initially
- [ ] Destroy key brick
- [ ] Exit activates immediately
- [ ] Progress shows "Keys: 1/1, Exit unlocked!"
- [ ] Destroy exit brick
- [ ] Level completes

### Expected Outcome
✅ Simplest GetTheKey configuration works
✅ Two-phase mechanic clear even with minimal setup

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 8: Keys and Exits with Variable Widths

### Setup
- GetTheKey level with wide key/exit bricks

### Test Steps
- [ ] 10-unit wide key brick works correctly
- [ ] 40-unit wide exit brick works correctly
- [ ] Collision detection accurate across full width
- [ ] Wide bricks activate/count same as narrow ones
- [ ] Visual appearance scales appropriately

### Expected Outcome
✅ Variable-width keys/exits work correctly
✅ No collision detection issues

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 9: Mixed with Regular Bricks

### Setup
- GetTheKey level with keys/exits blocked by regular bricks

### Test Steps
- [ ] Keys may be blocked by regular bricks
- [ ] Exits may be blocked by regular bricks
- [ ] Must destroy regular bricks to reach keys
- [ ] Must destroy regular bricks to reach exits
- [ ] Regular brick destruction doesn't affect key count
- [ ] Regular brick destruction doesn't activate exits
- [ ] Can complete level with some regular bricks remaining

### Expected Outcome
✅ Regular bricks provide obstacles but not requirements
✅ Strategic element in reaching keys and exits

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 10: Exit Activation Visual Feedback

### Setup
- Any GetTheKey level

### Test Steps
- [ ] Inactive exit has clear "locked" appearance
- [ ] Active exit has clear "unlocked" appearance
- [ ] Transition from inactive to active is noticeable
- [ ] Activation triggers sound effect (if audio implemented)
- [ ] HUD message confirms "Exit unlocked!"
- [ ] Player clearly understands state change

### Expected Outcome
✅ State transition is unmistakable
✅ Player never confused about exit status

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 11: Edge Cases

### Setup
- Various GetTheKey configurations

### Test Steps
- [ ] Level with many keys (10+) tracks correctly
- [ ] Level with many exits (5+) all activate together
- [ ] Keys destroyed in rapid succession handled correctly
- [ ] Exit destroyed same frame as last key collected handled correctly
- [ ] No crashes with unusual key/exit configurations

### Expected Outcome
✅ All configurations handled gracefully
✅ No edge case bugs or crashes

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Overall Test Summary

**Total Tests:** 11
**Passed:** ___
**Failed:** ___
**Pass Rate:** ___%

### Critical Issues Found
1. _____________________________________________
2. _____________________________________________

### Non-Critical Issues
1. _____________________________________________
2. _____________________________________________

### Recommendations
_____________________________________________

**Tester Signature:** _____________
**Date Completed:** _____________
