//
// macutils.mm
//  mac utils
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

int getHearthstonePID() {
    NSAutoreleasePool* autoReleasePool = [[NSAutoreleasePool alloc] init];

    NSArray* apps = [[NSWorkspace sharedWorkspace] runningApplications];

    NSRunningApplication* hsapp = NULL;
    for (NSRunningApplication* app in apps) {
        if ([[app bundleIdentifier] isEqualToString:@"unity.Blizzard Entertainment.Hearthstone"]) {
            hsapp = app;
            break;
        }
    }

    int result = 0;

    if (hsapp != NULL) {
        result = [hsapp processIdentifier];
    }

    [autoReleasePool release];

    return result;
}
