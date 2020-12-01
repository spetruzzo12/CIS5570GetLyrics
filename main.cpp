#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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
	ifstream in("Spotify2010_2019AllPop.txt");
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
	vector<string> songList;
	boolean runAPI = false;


	while (!in.fail()) {
		cout << "Processing Song #: " << numSongs << endl;

		getline(in, title, '\t');
		getline(in, artist, '\n');

		text = "";

		if (find(songList.begin(), songList.end(), title) != songList.end()) {
			runAPI = false;
			cout << "Song Already Found!" << endl;
		}
		else {
			runAPI = true;
			songList.push_back(title);
		}

		if (runAPI) {
			replaceSubString(title, " ", "%20");
			replaceSubString(artist, " ", "%20");

			url = "http://api.chartlyrics.com/apiv1.asmx/SearchLyricDirect?artist=" + artist + "&song=" + title;

			curl = curl_easy_init();

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &text);

			res = curl_easy_perform(curl);

			if ((startPos = text.find("Lyric /")) != string::npos) { 
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

			curl_easy_cleanup(curl);
			runAPI = false;
		} //end if API

		numSongs++;
	} //end while

	in.close();
	out.close();

	return 0;
}