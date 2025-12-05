# Manual Playtest Checklist: ReachForStars Levels

**Test Date:** _____________
**Tester:** _____________
**Build Version:** _____________

## Purpose

ReachForStars levels require destroying ONLY star bricks to complete. Regular bricks are optional. This checklist verifies that star collection gameplay works correctly.

---

## Pre-Test Setup

- [ ] Build is compiled and runs without errors
- [ ] Game launches successfully
- [ ] Can navigate to ReachForStars level
- [ ] Test ReachForStars level(s) are available

---

## Test 1: Star Collection Mechanics

### Setup
- Start ReachForStars level with multiple star bricks

### Test Steps
- [ ] Level displays mix of regular and star bricks
- [ ] Star bricks visually distinct from regular bricks (color/appearance)
- [ ] Can destroy star bricks by hitting with ball
- [ ] Can destroy regular bricks (but not required)
- [ ] Level completion progress shows "X/Y stars collected"
- [ ] HUD clearly indicates it's a ReachForStars level

### Expected Outcome
✅ Star bricks are clearly identifiable
✅ Both star and regular bricks can be destroyed
✅ Progress tracking shows star collection count

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 2: Level Completion (Stars Only)

### Setup
- Start ReachForStars level with 3 stars and 10 regular bricks

### Test Steps
- [ ] Level does NOT complete when regular bricks destroyed
- [ ] Level progress updates as each star is destroyed
- [ ] Level completes when ALL stars destroyed
- [ ] Regular bricks can remain on screen at completion
- [ ] Completion screen shows star collection achievement
- [ ] Score bonus awarded for collecting all stars

### Expected Outcome
✅ Level completion ONLY requires all stars destroyed
✅ Regular bricks are optional and don't prevent completion
✅ Clear feedback when level completes

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 3: Star Brick Variations

### Setup
- ReachForStars level with variable-width star bricks

### Test Steps
- [ ] 1-unit narrow star bricks work correctly
- [ ] 10+ unit wide star bricks work correctly
- [ ] 40-unit full-width star bricks work correctly
- [ ] All star brick widths count toward completion
- [ ] Collision detection accurate for all star brick sizes

### Expected Outcome
✅ Star bricks of all widths work correctly
✅ All stars count equally toward completion

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 4: Mixed Brick Strategy

### Setup
- ReachForStars level with stars blocked by regular bricks

### Test Steps
- [ ] Can destroy regular bricks to reach stars
- [ ] Destroying regular bricks doesn't affect star count
- [ ] Can strategically skip regular bricks to reach stars
- [ ] Ball physics work correctly with mixed brick patterns
- [ ] Can complete level without destroying all regular bricks

### Expected Outcome
✅ Player can choose strategy (destroy all or just stars)
✅ Level design allows multiple approaches

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 5: Single Star Level

### Setup
- ReachForStars level with only 1 star brick

### Test Steps
- [ ] Level displays correctly with single star
- [ ] Progress shows "0/1 stars"
- [ ] Level completes immediately when star destroyed
- [ ] Completion feedback mentions "Found the star!" or similar

### Expected Outcome
✅ Single star level works correctly
✅ Provides appropriate challenge/feedback

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 6: Multiple Stars Scattered

### Setup
- ReachForStars level with 5+ stars scattered across field

### Test Steps
- [ ] All stars visible and distinguishable
- [ ] Progress counter updates correctly (0/5, 1/5, 2/5, etc.)
- [ ] Can destroy stars in any order
- [ ] Missing one star prevents completion
- [ ] Final star destruction triggers completion
- [ ] Completion shows total stars collected

### Expected Outcome
✅ Multi-star collection tracked accurately
✅ Clear visual and numerical feedback

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 7: Star Collection Progress Display

### Setup
- Any ReachForStars level

### Test Steps
- [ ] HUD shows "Stars: X/Y" or similar
- [ ] Counter updates immediately when star destroyed
- [ ] Counter position is clear and readable
- [ ] Counter distinguishes from score/lives display
- [ ] Level type clearly marked as "Reach for the Stars"

### Expected Outcome
✅ Progress display is clear and informative
✅ Player always knows completion status

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 8: ReachForStars vs Classical Comparison

### Setup
- Play both Classical and ReachForStars levels consecutively

### Test Steps
- [ ] Can distinguish level types from HUD/UI
- [ ] Completion criteria clearly different
- [ ] Star bricks behave same in both level types
- [ ] Classical requires all bricks, ReachForStars only stars
- [ ] Player understands difference without confusion

### Expected Outcome
✅ Level types are clearly differentiated
✅ No confusion about completion criteria

**Result:** [ ] PASS [ ] FAIL

**Notes:**
_______________________________________________

---

## Test 9: Edge Cases

### Setup
- Various ReachForStars configurations

### Test Steps
- [ ] Level with no regular bricks (stars only) works correctly
- [ ] Level with many regular bricks, few stars works correctly
- [ ] Level with all bricks as stars works like Classical
- [ ] Star brick destruction sequence doesn't matter
- [ ] No crashes with unusual star configurations

### Expected Outcome
✅ All valid configurations work correctly
✅ Edge cases handled gracefully

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

### Non-Critical Issues
1. _____________________________________________
2. _____________________________________________

### Recommendations
_____________________________________________

**Tester Signature:** _____________
**Date Completed:** _____________
