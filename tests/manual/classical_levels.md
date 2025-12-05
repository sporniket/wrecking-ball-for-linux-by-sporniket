# Manual Playtest Checklist: Classical Levels

**Test Date:** _____________
**Tester:** _____________
**Build Version:** _____________

## Purpose

Classical levels require destroying ALL bricks to complete the level. This checklist verifies that Classical level gameplay works correctly.

---

## Pre-Test Setup

- [ ] Build is compiled and runs without errors
- [ ] Game launches successfully
- [ ] Can navigate to level selection or start game
- [ ] Test Classical level(s) are available

---

## Test 1: Basic Classical Level Gameplay

### Setup
- Start Classical level 1 (easy difficulty)

### Test Steps
- [ ] Level loads correctly with bricks displayed
- [ ] Paddle is visible at bottom of screen
- [ ] Ball starts attached to paddle or launches immediately
- [ ] Can control paddle with mouse (moves left/right)
- [ ] Can control paddle with keyboard (arrow keys or A/D)
- [ ] Ball launches when expected (space/click)

### Brick Destruction
- [ ] Ball collides with bricks and bounces correctly
- [ ] Bricks take damage when hit (visual feedback)
- [ ] 1-hit bricks destroy immediately
- [ ] Multi-hit bricks change appearance after each hit
- [ ] Multi-hit bricks destroy after all hits depleted
- [ ] Score increases when bricks are destroyed
- [ ] Destroyed bricks disappear from play field

### Expected Outcome
✅ All bricks can be destroyed
✅ Level completion triggers when all bricks destroyed
✅ Completion screen or next level prompt appears

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 2: Variable-Width Brick Handling

### Setup
- Start Classical level with variable-width bricks

### Test Steps
- [ ] 1-unit narrow bricks are visible and hittable
- [ ] 10+ unit wide bricks are visible and hittable
- [ ] 40-unit full-width bricks are visible and hittable
- [ ] Ball collides correctly with left edge of wide bricks
- [ ] Ball collides correctly with center of wide bricks
- [ ] Ball collides correctly with right edge of wide bricks
- [ ] Wide bricks destroy same as narrow bricks (same hits required)

### Expected Outcome
✅ All brick widths (1, 10+, 40 units) work correctly
✅ Collision detection accurate across full brick width
✅ Level completion works regardless of brick widths

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 3: Paddle Bounce Physics

### Setup
- Start any Classical level

### Test Steps
- [ ] Ball bounces straight up when hitting paddle center
- [ ] Ball bounces left when hitting left side of paddle
- [ ] Ball bounces right when hitting right side of paddle
- [ ] Ball bounce angle changes based on impact position
- [ ] Ball speed remains consistent after paddle bounce
- [ ] Ball can be aimed by moving paddle during contact

### Expected Outcome
✅ Paddle bounce physics feel responsive and predictable
✅ Player has control over ball direction via paddle position

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 4: Wall and Ceiling Bounces

### Setup
- Play any Classical level

### Test Steps
- [ ] Ball bounces correctly off left wall
- [ ] Ball bounces correctly off right wall
- [ ] Ball bounces correctly off ceiling/top
- [ ] Ball angle preserved after wall bounces (elastic collision)
- [ ] Ball speed preserved after wall bounces
- [ ] No ball stuck in walls or ceiling

### Expected Outcome
✅ Ball physics work correctly on all boundaries
✅ Ball never gets stuck or behaves erratically

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 5: Ball Loss Detection (Casual Mode)

### Setup
- Start Classical level in Casual mode

### Test Steps
- [ ] Ball detected as lost when it falls below paddle
- [ ] New ball spawns immediately after loss (Casual mode)
- [ ] Ball attaches to paddle after respawn
- [ ] Ball can be launched again normally
- [ ] Level progress preserved (destroyed bricks stay destroyed)
- [ ] Lives counter does NOT decrease (Casual mode)

### Expected Outcome
✅ Ball respawn works correctly in Casual mode
✅ Player can continue playing indefinitely

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 6: Level Completion Detection

### Setup
- Play Classical level to near completion

### Test Steps
- [ ] Level completion detected when last brick destroyed
- [ ] Completion screen appears immediately after last brick
- [ ] Final score displayed correctly
- [ ] Completion message indicates Classical level type
- [ ] Can proceed to next level or return to menu
- [ ] Progress is saved (level unlocked, score recorded)

### Expected Outcome
✅ Level completion triggers at correct moment
✅ Completion UI provides clear feedback
✅ Player can continue or exit gracefully

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 7: Pause and Resume

### Setup
- Play any Classical level

### Test Steps
- [ ] Can pause game (ESC or pause button)
- [ ] Game freezes when paused (ball, paddle stop)
- [ ] Pause menu appears with options
- [ ] Can resume game from pause menu
- [ ] Game continues from exact state before pause
- [ ] Can exit to menu from pause menu

### Expected Outcome
✅ Pause/resume functionality works correctly
✅ No gameplay state lost during pause

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 8: Performance and Stability

### Setup
- Play multiple Classical levels continuously

### Test Steps
- [ ] Game maintains smooth 60 FPS
- [ ] No frame drops during intense brick destruction
- [ ] No memory leaks over extended play (10+ minutes)
- [ ] No crashes or freezes
- [ ] Audio and visual effects stay synchronized
- [ ] Input remains responsive throughout session

### Expected Outcome
✅ Game runs stably for extended periods
✅ Performance remains consistent

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 9: Edge Cases

### Setup
- Various Classical levels

### Test Steps
- [ ] Empty level (no bricks) completes immediately
- [ ] Level with single brick completes when destroyed
- [ ] Level with 100+ bricks handles correctly
- [ ] Ball collision with corner bricks works correctly
- [ ] Multiple simultaneous brick collisions handled
- [ ] Rapid paddle movement doesn't break physics

### Expected Outcome
✅ Game handles edge cases without errors
✅ No unexpected behavior or crashes

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Overall Test Summary

**Total Tests:** 9
**Passed:** ___
**Failed:** ___
**Pass Rate:** ___%

### Critical Issues Found
1. _____________________________________________
2. _____________________________________________
3. _____________________________________________

### Non-Critical Issues
1. _____________________________________________
2. _____________________________________________

### Recommendations
_____________________________________________
_____________________________________________

**Tester Signature:** _____________
**Date Completed:** _____________
