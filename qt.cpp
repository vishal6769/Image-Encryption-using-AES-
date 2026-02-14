#import <Cocoa/Cocoa.h>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    @autoreleasepool {

        NSSavePanel *panel = [NSSavePanel savePanel];
        [panel setTitle:@"Save Your File"];
        [panel setMessage:@"Choose where to save the file"];
        [panel setAllowedFileTypes:@[@"txt"]]; // allow saving as .txt
        [panel setAllowsOtherFileTypes:YES];

        // Show popup dialog
        if ([panel runModal] == NSModalResponseOK) {

            NSString *path = [[panel URL] path];
            std::string filePath = [path UTF8String];

            std::cout << "User chose to save at: " << filePath << "\n";

            // Write example content
            std::ofstream out(filePath);
            out << "This is saved using NSSavePanel!";
            out.close();

            std::cout << "File saved successfully.\n";
        } else {
            std::cout << "User cancelled.\n";
        }
    }
    return 0;
}
