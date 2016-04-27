#import "MGLUITestCase.h"

@interface MGLGestureTests : MGLUITestCase

@end

@implementation MGLGestureTests

- (void)testRotate {
    XCUIElement *mapElement = [[XCUIApplication alloc] init].otherElements[@"MGLMapView"];
    XCUIElement *compassElement = mapElement.buttons[@"MGLMapViewCompass"];
    NSPredicate *unhittable = [NSPredicate predicateWithFormat:@"hittable == NO"];
    
    XCTAssertFalse(compassElement.hittable, @"Compass should be absent when the map is unrotated.");
    
    // Attempt to rotate the map 90° counterclockwise. The map should unrotate itself.
    [mapElement rotate:M_PI_2 withVelocity:M_PI];
    XCTAssertTrue(compassElement.hittable, @"Compass should be present when the map is rotated.");
    XCTAssertNotEqualWithAccuracy([compassElement.value rangeOfString:@"west"].location, NSNotFound, 1, @"Map should point west or thereabouts after rotating 90° counterclockwise. Instead, it points %@.", compassElement.value);
    [self expectationForPredicate:unhittable evaluatedWithObject:compassElement handler:nil];
    [self waitForExpectationsWithTimeout:3 handler:nil];
    
    // Zoom in until rotation is allowed.
    [mapElement doubleTap];
    [mapElement doubleTap];
    [mapElement doubleTap];
    
    // Rotate the map 90° counterclockwise.
    [mapElement rotate:M_PI_2 withVelocity:M_PI];
    XCTAssertTrue(compassElement.hittable, @"Compass should be present when the map is rotated.");
    XCTAssertNotEqualWithAccuracy([compassElement.value rangeOfString:@"west"].location, NSNotFound, 1, @"Map should point west or thereabouts after rotating 90° counterclockwise. Instead, it points %@.", compassElement.value);
    
    // Keep interacting with the map to make sure it doesn’t unrotate.
    [mapElement swipeLeft];
    [mapElement swipeRight];
    
    // Unrotate the map.
    [compassElement tap];
    [self expectationForPredicate:unhittable evaluatedWithObject:compassElement handler:nil];
    [self waitForExpectationsWithTimeout:3 handler:nil];
}

@end