# CIS5570GetLyrics

Download LibCURL First (need Visual Studio 19):

Get latest vcpkg zip file from https://github.com/microsoft/vcpkg/releases (e.g. https://github.com/microsoft/vcpkg/archive/2019.09.zip) and extract it to a folder of your choice (e.g. C:\vcpkg\)

Open Developer Command Prompt for VS 2017 (see Windows Start menu or %PROGRAMDATA%\Microsoft\Windows\Start Menu\Programs\Visual Studio 2017\Visual Studio Tools\) and cd to C:\vcpkg\

Run bootstrap-vcpkg.bat

Run vcpkg.exe integrate install

Run vcpkg.exe install curl

Create a new C++ project in Visual Studio and you're ready to go - try it with the example below. There's no need to modify project settings.

#define CURL_STATICLIB
#include <curl\curl.h>

int main()
{
    CURL *curl;

    curl = curl_easy_init();
    curl_easy_cleanup(curl);

    return 0;
}

Then create a new project and add the main.cpp to the source files, output.txt to the resource files, then an input text file with the song title and artist in a tab separated list (RollingStones500TitleAndArtist.txt) was added for this purpose.

Make sure line 25, where it has "ifstream in()" has your text file name in between the parathesis with ".txt" added to the file name.

Click the green arrow at the top of Visual Studios to run the program.

The output should list when the Lyrics cannot be found of if there is an error.

