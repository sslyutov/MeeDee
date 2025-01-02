//
//  utils.h
//  MeeDee
//
//  Created by Sergey Slyutov on 1/1/25.
//
#include <QString>
#include <CoreFoundation/CoreFoundation.h>

QString CFStringToQString(CFStringRef cfString) {
    if (!cfString) {
        return QString();
    }

    // Get the length of the CFString
    CFIndex length = CFStringGetLength(cfString);

    // Allocate a buffer for UTF-16 characters
    std::unique_ptr<UniChar[]> buffer(new UniChar[length]);

    // Copy the UTF-16 characters into the buffer
    CFStringGetCharacters(cfString, CFRangeMake(0, length), buffer.get());

    // Create a QString from the UTF-16 data
    return QString(reinterpret_cast<const QChar*>(buffer.get()), static_cast<int>(length));
}

