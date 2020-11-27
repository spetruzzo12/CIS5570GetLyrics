#include <iostream>
#include <fstream>
#include <string>
#include <curl\curl.h>

using namespace std;

#define CURL_STATICLIB

void replaceSubString(string& subject, const string& search, const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

size_t write(void* ptr, size_t size, size_t nmemb, string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

int main(int argc, char** argv)
{
	ifstream in("RollingStones500TitleAndArtist.txt");
	ofstream out("output.txt");

	int numSongs = 1;
	string title;
	string artist;
	CURL* curl;
	CURLcode res;
	string text = "";
	string url = "";
	size_t startPos;
	size_t endPos;
	size_t lyricDirectPos;

	while (!in.fail()) {
		getline(in, title, '\t');
		getline(in, artist, '\n');

		text = "";

		replaceSubString(title, " ", "%20");
		replaceSubString(artist, " ", "%20");

		url = "http://api.chartlyrics.com/apiv1.asmx/SearchLyricDirect?artist=" + artist + "&song=" + title;

		cout << "Processing Song #: " << numSongs << endl;

		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);

		res = curl_easy_perform(curl);

		if ((startPos = text.find("Lyric /")) != string::npos) { //does this need to be npos or not???
			cout << "No lyrics found for this song!" << endl;
		}
		else if ((startPos = text.find("<Lyric>", 0) + 7) != string::npos && (endPos = text.find("</Lyric>", 0)) != string::npos) {
			text = text.substr(startPos, endPos - startPos);

			if ((lyricDirectPos = text.find("LyricDirect", 0)) != string::npos)
				text = text.substr(0, lyricDirectPos);

			out << text << endl;
		}
		else
			cout << "FAILED! Title: " + title + " Artist: " + artist << endl;

		numSongs++;

		curl_easy_cleanup(curl);
	}

	in.close();
	out.close();

	return 0;
}