#include "imp.h"

struct song_node *first;
struct song_node *firstChr;
struct song_node *firstSrt;
struct song_node *firstRan;

using namespace std;

void list::openDatabase() {

	fp = fopen("songbook.txt", "r");
	if(!fp) {
		cout << "Cannot open the database file" << endl;
		return;
	}

	rewind(fp);
}

void list::readFromDatabase() {
	struct song buffer;
	char line[100];
	openDatabase();

	for (num_songs = 0; !feof(fp); num_songs++) {
		if(fgets(line, 255, fp) == NULL)
			break;
		if(strcmp(line, "*****\r\n") == 0)
			break;
	}

	rewind(fp);

	first    = NULL;
	firstChr = NULL;
	firstSrt = NULL;
	firstRan = NULL;

	for(int count = 0; count < num_songs; count++) {

		fscanf(fp, "%[^\t]\t%[^\t]\t%d\t%s", buffer.name, buffer.singer,
											 &buffer.year,buffer.style);

		if (buffer.name[0] == '\r' && buffer.name[1] == '\n') {
			int check = strlen(buffer.name);
			for (int j = 0; j < (check - 1); j++) {
				buffer.name[j] = buffer.name[j+2];
				buffer.name[j+2] = '\0';
			}
		}

		insertNodeWOSort(buffer);
		insertNodeChrnlgcl(buffer);
		insertNodeSorted(buffer);
	}

	createRandomList();

	initializePlaylists();

	fclose(fp);
}

void list::songPrinter(song_node *node) {

	struct song_node *index = node;
	for (int count = 0; count < (node->parent)->songnumber; count++) {
		cout << setw(2) << right << count+1 << ". ";
		cout << setw(40) << left << (index->data)->name << " | ";
		cout << setw(20) << left << (index->data)->singer << " | ";
		cout << setw(4) << (index->data)->year << " | ";
		cout << setw(20) << left << (index->data)->style << endl;
		index = index->next;
	}
}

void list::copyString(char **dest, char *src) {

	delete [] *dest;

	int new_length = strlen(src) + 1;
	*dest = new char[new_length];
	strcpy(*dest, src);
}

void list::insertNodeWOSort(song buffer) {
	song_node *last;
	song_node *new_node = new song_node;
	song *data = new song;

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;

	if (first == NULL)
		first = new_node;

	else {

		if (first->prev != NULL)
			last = first->prev;
		else
			last = first;

		last->next = new_node;
		new_node->prev = last;
		last = new_node;
		first->prev = last;
		last->next = first;
	}
}

void list::insertNodeChrnlgcl(song buffer){
	song_node *lastChr;
	song_node *new_node = new song_node;
	song *data = new song;

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;

	if (firstChr == NULL) {
		firstChr = new_node;
		return;
	}


	if (firstChr->prev != NULL)
		lastChr = firstChr->prev;
	else
		lastChr = firstChr;

	song_node *position = firstChr;
	song_node *tail = NULL;

	do {

		if (data->year < (position->data)->year) {

			if (position == firstChr) {
				new_node->next = firstChr;
				firstChr->prev = new_node;
				firstChr = new_node;
				firstChr->prev = lastChr;
				lastChr->next = firstChr;
				return;
			}
			new_node->next = position;
			new_node->prev = tail;
			position->prev = new_node;
			tail->next = new_node;
			return;
		}
		tail = position;
		position = position->next;

	}
	while (firstChr->next != NULL && position != firstChr);

	lastChr->next = new_node;
	new_node->prev = lastChr;
	lastChr = new_node;
	firstChr->prev = lastChr;
	lastChr->next = firstChr;
}

void list::insertNodeSorted(song buffer) {
	song_node *lastSrt;
	song_node *new_node = new song_node;
	song *data = new song;

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	copyString(&(data->name),   buffer.name);
	copyString(&(data->singer), buffer.singer);
	data->year = buffer.year;
	copyString(&(data->style),  buffer.style);

	new_node->data = data;

	if (firstSrt == NULL) {
		firstSrt = new_node;
		return;
	}

	if (firstSrt->prev != NULL)
		lastSrt = firstSrt->prev;
	else
		lastSrt = firstSrt;

	song_node *position = firstSrt;
	song_node *tail = NULL;

	do {

		if (strcmp(data->singer, (position->data)->singer) < 0) {
			if (position == firstSrt) {
				new_node->next = firstSrt;
				firstSrt->prev = new_node;
				firstSrt = new_node;
				firstSrt->prev = lastSrt;
				lastSrt->next = firstSrt;
				return;
			}

			new_node->next = position;
			new_node->prev = tail;
			position->prev = new_node;
			tail->next = new_node;
			return;
		}

		else if (strcmp(data->singer, (position->data)->singer) == 0) {
			if (strcmp(data->name, (position->data)->name) < 0) {
				new_node->next = position;
				new_node->prev = tail;
				position->prev = new_node;
				tail->next = new_node;
				return;
			}
		}

		tail = position;
		position = position->next;

	} while(firstSrt->next != NULL && position != firstSrt);

	lastSrt->next = new_node;
	new_node->prev = lastSrt;
	lastSrt = new_node;
	firstSrt->prev = lastSrt;
	lastSrt->next = firstSrt;
}

void list::insertNodeRandom(int random_num) {
	song_node *lastRan;
	song_node *node;
	song_node *new_node = new song_node;
	song *data = new song;

	node = first;
	for (int count = 0; count < random_num; count++) {
		node = node->next;
	}

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;

	copyString(&(data->name),   (node->data)->name);
	copyString(&(data->singer), (node->data)->singer);
	data->year = (node->data)->year;
	copyString(&(data->style),  (node->data)->style);

	new_node->data = data;

	if (firstRan == NULL)
		firstRan = new_node;

	else {
		if (firstRan->prev != NULL)
			lastRan = firstRan->prev;
		else
			lastRan = firstRan;

		lastRan->next = new_node;
		new_node->prev = lastRan;
		lastRan = new_node;
		firstRan->prev = lastRan;
		lastRan->next = firstRan;
	}
}

void list::createRandomList() {

	int *ranMemory = new int [num_songs];
	int random_num, num_inserted;

	struct playlist *control = NULL;

	if (firstRan != NULL) {

		if (playlist_count != 0)
			control = firstRan->parent;

		struct song_node *current = firstRan;
		struct song_node *next = firstRan;
		struct song *data;

		for (int count = 0; count < num_songs; count ++) {
			current = next;

			if (count != num_songs - 1)
				next = current->next;

			data = current->data;

			delete [] data->name;
			delete [] data->singer;
			delete [] data->style;
			delete data;

			delete current;

		}
		firstRan = NULL;
	}

	srand(time(NULL));
	random_num = rand() % num_songs;

	for(num_inserted = 0; num_inserted < num_songs; num_inserted++) {

		for (int count = 0; count < num_inserted;) {

			if (random_num == *(ranMemory+count)) {
				random_num = rand() % num_songs;
				count = 0;
			}
			else
				count++;
		}

		*(ranMemory+num_inserted) = random_num;
		insertNodeRandom(random_num);
	}

	delete [] ranMemory;

	if (control != NULL) {
		control->head = firstRan;

		struct song_node *current = firstRan;
		for (int count = 0; count < control->songnumber; count++) {
			current->parent = control;
			current = current->next;
		}
	}
}

void list::initializePlaylists() {
	struct song_node *check = NULL;

	struct playlist *chr = new playlist;

	chr->songnumber = num_songs;
	delete [] chr->name;
	chr->name = new char [strlen("CHRONOLOGICAL")+1];
	strcpy(chr->name, "CHRONOLOGICAL");
	chr->head = firstChr;
	playlist_count = 1;

	check = firstChr;
	for (int count = 0; count < num_songs; count++) {
		check->parent = chr;
		check = check->next;
	}

	struct playlist *srt = new playlist;

	srt->songnumber = num_songs;
	delete [] srt->name;
	srt->name = new char [strlen("SORTED")+1];
	strcpy(srt->name, "SORTED");
	srt->head = firstSrt;
	playlist_count++;

	check = firstSrt;
	for (int count = 0; count < num_songs; count++) {
		check->parent = srt;
		check = check->next;
	}

	struct playlist *ran = new playlist;

	ran->songnumber = num_songs;
	delete [] ran->name;
	ran->name = new char [strlen("RANDOM")+1];
	strcpy(ran->name, "RANDOM");
	ran->head = firstRan;
	playlist_count++;

	check = firstRan;
	for (int count = 0; count < num_songs; count++) {
		check->parent = ran;
		check = check->next;
	}

	head = chr;
	head->next = srt;
	srt->prev = head;
	srt->next = ran;
	ran->prev = srt;
	ran->next = head;
	head->prev = ran;
}

void list::createList() {
	struct playlist *lastList;
	struct playlist *new_list = new playlist;

	new_list->head = NULL;
	new_list->songnumber = 0;

	cout << endl << "NEW PLAYLIST >" << endl;
	cout << "1. Create with songs of a specific style" << endl;
	cout << "2. Create with songs of a specific singer" << endl;
	cout << "3. Create with a combination of existing playlists" << endl;
	cout << "4. Create with a combination of existing songs" << endl;
	cout << endl << "Enter a choice (1, 2, 3, 4): ";

	int choice;
	cin >> choice;

	if (choice == 1) {
		struct song_node *check = first;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		char style[STYLE_LENGTH];

		cout << "Enter the style: ";

		while (getchar() != '\n');
		scanf("%[^\n]", style);

		for (int count = 0; count < num_songs; count++) {
			if (strcmp((check->data)->style, style) == 0) {
				createNode(&firstNew, &lastNew, check);
				(new_list->songnumber)++;
			}
			check = check->next;
		}

		copyString(&(new_list->name), style);
		new_list->head = firstNew;
	}

	else if (choice == 2) {
		struct song_node *check = first;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		char singer[SINGER_LENGTH];

		cout << "Enter the singer: ";
		while (getchar() != '\n');
		scanf("%[^\n]", singer);

		for (int count = 0; count < num_songs; count++) {
			if (strcmp((check->data)->singer, singer) == 0) {
				createNode(&firstNew, &lastNew, check);
				(new_list->songnumber)++;
			}
			check = check->next;
		}

		copyString(&(new_list->name), singer);
		new_list->head = firstNew;
	}

	else if (choice == 3) {
		struct playlist *check = NULL;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;
		struct song_node *insert = NULL;
		int selection;

		do {
			cout << endl;
			check = head;
			for (int count = 0; count < playlist_count; count++) {
				cout << count+1 << ". ";
				cout << check->name << endl;
				check = check->next;
			}

			cout << endl << "Playlist to add ('-1' to end): ";
			cin >> selection;

			if (selection == -1)
				break;

			check = head;
			for (int count = 1; count < selection; count++) {
				check = check->next;
			}
			new_list->songnumber += check->songnumber;

			insert = check->head;
			for (int count = 0; count < check->songnumber; count++) {
				createNode(&firstNew, &lastNew, insert);
				insert = insert->next;
			}

			if (new_list->head == NULL)
				new_list->head = firstNew;


		} while (selection != -1);
		if (new_list->head != NULL) {
			cout << "Enter name of the playlist: ";
			while (getchar() != '\n');
			scanf("%[^\n]",new_list->name);
		}
	}

	else if (choice == 4) {
		struct song_node *check = NULL;
		struct song_node *firstNew = NULL;
		struct song_node *lastNew = NULL;

		int selection;

		do {

			cout << endl;
			songPrinter(firstSrt);
			cout << endl << "Song to add ('-1' to end): ";
			cin >> selection;

			if (selection == -1)
				break;

			check = firstSrt;
			for (int count = 1; count < selection; count++) {
				check = check->next;
			}

			(new_list->songnumber)++;

			createNode(&firstNew, &lastNew, check);

			if (new_list->head == NULL)
				new_list->head = firstNew;

		} while (selection != -1);

		if (new_list->head != NULL) {
			cout << "Enter name of the playlist: ";
			while (getchar() != '\n');
			scanf("%[^\n]",new_list->name);
		}
	}

	else {
		cout << endl << "Wrong input... " << endl;
		delete new_list;
		return;
	}

	if (new_list->songnumber == 0) {
		delete new_list;
		return;
	}

	lastList = head->prev;
	new_list->prev = lastList;
	lastList->next = new_list;
	lastList = new_list;
	head->prev = lastList;
	lastList->next = head;
	playlist_count++;

	struct song_node *index = new_list->head;
	for (int count = 0; count < new_list->songnumber; count++) {
		index->parent = new_list;
		index = index->next;
	}
	cout << endl;
	songPrinter(new_list->head);
}

void list::createNode(song_node **start, song_node **end, song_node *insert) {

	song_node *new_node = new song_node;

	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->parent = NULL;
	new_node->data = insert->data;

	if (*start == NULL)
		*start = *end = new_node;

	else {
		(*end)->next = new_node;
		new_node->prev = *end;
		*end = new_node;
		(*start)->prev = *end;
		(*end)->next = *start;
	}
}

void list::addSong(int position) {
	int song_selection, playlist_selection, diff, order = 1;
	bool is_user_added_playlist = false;
	struct playlist *check = head;

	if (position == -1) {
		cout << endl;
		for (int count = 0; count < playlist_count; count++) {
			if (strcmp(check->name, "CHRONOLOGICAL") != 0 &&
				strcmp(check->name, "SORTED") != 0  &&
				strcmp(check->name, "RANDOM") != 0) {

				cout << order << ". " << check->name << endl;
				order++;
				is_user_added_playlist = true;
			}

			check = check->next;
		}

		if (!is_user_added_playlist) {
			cout << endl << "There is no user added list." << endl;
			return;
		}

		cout << "Select a playlist: ";
		cin >> playlist_selection;
		diff = 2;
	}
	else {
		playlist_selection = position - 1;
		diff = 0;
	}

	cout << endl;
	songPrinter(firstSrt);

	cout << "Song to add: ";
	cin >> song_selection;

	struct song_node *index = firstSrt;
	for (int count = 1; count < song_selection; count++) {
		index = index->next;
	}

	for (int count = 0; count < playlist_selection + diff; count++) {
		check = check->next;
	}

	song buffer;
	copyString(&(buffer.name), (index->data)->name);
	copyString(&(buffer.singer), (index->data)->singer);
	buffer.year = (index->data)->year;
	copyString(&(buffer.style), (index->data)->style);

	if (check->head == firstChr)
		insertNodeChrnlgcl(buffer);

	else if (check->head == firstSrt)
		insertNodeSorted(buffer);

	else if (strcmp(check->name, "RANDOM") == 0) {
		insertNodeWOSort(buffer);
		num_songs++;
		createRandomList();
	}

	else {
		createNode(&(check->head), &((check->head)->prev), index);
		((check->head)->prev)->parent = check;
	}

	(check->songnumber)++;

	cout << endl;
	songPrinter(check->head);

	cout << endl << "Song added." << endl;
}

void list::play(char opt, int sel) {
	char choice;
	struct song_node *play;
	createRandomList();
	if (opt == '*') {
		cout << endl << "PLAY >" << endl;
		cout << "Choose an option" << endl;
		cout << "L: Play a playlist" << endl;
		cout << "S: Play a playlist starting from a specific song" << endl;
		cout << "P: Play a single song" << endl;
		cout << "E: Exit" << endl;

		cout << endl << "Enter a choice: ";
		cin >> choice;
	}

	else
		choice = opt;

	if (choice == 'L') {
		int sel_list;

		struct playlist *check = head;
		if (sel == -1) {
			for (int count = 0; count < playlist_count; count++) {
				cout << count+1 << ". " << check->name << endl;
				check = check->next;
			}

			cout << endl << "Select a playlist: ";
			cin >> sel_list;
		}

		else
			sel_list = sel;

		for (int count = 0; count < sel_list - 1; count++) {
			check = check->next;
		}

		play = check->head;
	}

	else if (choice == 'S') {
		int sel_list, sel_song;

		struct playlist *check = head;
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << check->name << endl;
			check = check->next;
		}

		cout << endl << "Select a playlist: ";
		cin >> sel_list;

		for (int count = 0; count < sel_list-1; count++) {
			check = check->next;
		}

		cout << endl;
		songPrinter(check->head);

		cout << endl << "Select a song: ";
		cin >> sel_song;

		struct song_node *index = check->head;
		for (int count = 1; count < sel_song; count++) {
			index = index->next;
		}

		play = index;
	}

	else if (choice == 'P') {
		int sel_song;

		cout << endl;
		songPrinter(firstSrt);

		cout << endl << "Select a song: ";
		cin >> sel_song;

		struct song_node *index = firstSrt;
		for (int count = 1; count < sel_song; count++) {
			index = index->next;
		}

		cout << "playing..." << endl;
		cout << (index->data)->name << " - ";
		cout << (index->data)->singer << " - ";
		cout << (index->data)->style << endl;

		return;
	}

	else if (choice == 'E')
		return;

	else {
		cout << endl << "Wrong input." << endl << endl;
		return;
	}

	while (choice != 'E') {
		cout << "playing..." << endl;
		cout << (play->data)->name << " - ";
		cout << (play->data)->singer << " - ";
		cout << (play->data)->style << endl;
		cout << "NEXT(N) - PREVIOUS(P) - EXIT(E)" << endl;
		while (getchar() != '\n');
		choice = getchar();

		if (choice == 'N')
			play = play->next;

		else if (choice == 'P')
			play = play->prev;

		else if (choice == 'E')
			return;

		else
			cout << "Wrong input." << endl;
	}
}

void list::removeSong(int sel) {
	int sel_list, sel_song;
	struct playlist *check = head;

	if (sel == -1) {
		cout << endl << "REMOVE SONG >" << endl;
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << check->name << endl;
			check = check->next;
		}
		cout << endl << "Select a playlist: ";
		cin >> sel_list;
	}

	else
		sel_list = sel;
	for (int count = 0; count < sel_list-1; count++) {
		check = check->next;
	}
	cout << endl;
	songPrinter(check->head);
	cout << endl << "Select a song: ";
	cin >> sel_song;

	struct song_node *index = check->head;
	for (int count = 1; count < sel_song; count++) {
		index = index->next;
	}
	(index->prev)->next = index->next;
	(index->next)->prev = index->prev;

	if ((index->parent)->head == index)
		(index->parent)->head = index->next;
	((index->parent)->songnumber)--;
	delete index;

	cout << endl << "Song has been removed." << endl;
}

void list::deleteList(int sel) {
	int sel_list;
	struct playlist *index = head;
	if (sel == -1) {
		cout << endl << "DELETE PLAYLIST >" << endl;
		for (int count = 0; count < playlist_count; count++) {
			cout << count+1 << ". " << index->name << endl;
			index = index->next;
		}
		cout << "Select a playlist: ";
		cin >> sel_list;
	}

	else
		sel_list = sel;

	for (int count = 0; count < sel_list-1; count++) {
		index = index->next;
	}

	(index->prev)->next = index->next;
	(index->next)->prev = index->prev;

	if (index == head)
		head = index->next;

	struct song_node *next_position = index->head;
	struct song_node *position;
	for (int count = 0; count < index->songnumber; count++) {
		position = next_position;

		next_position = position->next;

		delete position;
	}

	delete [] index->name;
	delete index;

	playlist_count--;

	cout << endl << "Playlist has been deleted." << endl;
}

void list::printList() {
	int sel_list;
	struct playlist *check;
	char choice;
	cout << endl << "PRINT PLAYLIST >" << endl;
	check = head;
	for (int count = 0; count < playlist_count; count++) {
		cout << count+1 << ". " << check->name << endl;
		check = check->next;
	}

	cout << endl << "Select a playlist: ";
	cin >> sel_list;

	for (int count = 0; count < sel_list-1; count++) {
		check = check->next;
	}

	do {
		cout << endl;
		cout << sel_list << ". " << check->name << endl;

		cout << "NEXT(N) - PREVIOUS(P) - SELECT(S) - EXIT(E)" << endl;
		while (getchar() != '\n');
		choice = getchar();

		if (choice == 'N') {
			check = check->next;
			if (sel_list == playlist_count)
				sel_list = 1;
			else
				sel_list++;
		}

		else if (choice == 'P') {
			check = check->prev;
			if (sel_list == 1)
				sel_list = playlist_count;
			else
				sel_list--;
		}

		else if (choice == 'S') {
			char option;
			cout << "DELETE THE PLAYLIST(D) - ADD SONG(A) - ";
			cout << "REMOVE SONG(R) - PLAY(P)" << endl;
			while (getchar() != '\n');
			option = getchar();

			if (option == 'D')
				deleteList(sel_list);

			else if (option == 'A')
				addSong(sel_list);

			else if (option == 'R')
				removeSong(sel_list);

			else if (option == 'P')
				play('L', sel_list);

			else
				cout << endl << "Wrong input." << endl << endl;

			return;
		}

		else if (choice == 'E')
			return;

		else {
			cout << endl << "Wrong input." << endl << endl;
			return;
		}

		createRandomList();
	} while (choice != 'E');
}

void list::writeToFile() {
	struct playlist  *current_list;
	struct song_node *node;

	fp = fopen("songbook.txt", "w");

	current_list = head;

	for (int count = 0; count < playlist_count; count++) {

		if (!(strcmp(current_list->name, "CHRONOLOGICAL") == 0 ||
			  strcmp(current_list->name, "SORTED") == 0 ||
			  strcmp(current_list->name, "RANDOM") == 0)) {
				fprintf(fp, "*****\r\n%s\r\n", current_list->name);
		}

		node = current_list->head;

		if (!(strcmp(current_list->name, "SORTED") == 0 ||
			  strcmp(current_list->name, "RANDOM") == 0)) {

			for (int count2 = 0; count2 < current_list->songnumber; count2++) {
				fprintf(fp, "%s\t%s\t%d\t%s\r\n",(node->data)->name,
												 (node->data)->singer,
												 (node->data)->year,
												 (node->data)->style);
				node = node->next;
			}
		}

		current_list = current_list->next;
	}

	cout << endl << "Playlists has been written to file." << endl;
	fclose(fp);
}

void list::exit() {

	struct playlist  *current_list;
	struct playlist  *next_list;
	struct song_node *node;
	struct song_node *next_node;
	struct song      *data;
	bool is_user_added_playlist;

	writeToFile();
	current_list = next_list = head;
	for (int count = 0; count < playlist_count; count++) {
		current_list = next_list;

		if (count != playlist_count - 1)
			next_list = current_list->next;

		if (strcmp(current_list->name, "CHRONOLOGICAL") == 0 ||
			strcmp(current_list->name, "SORTED") == 0 ||
			strcmp(current_list->name, "RANDOM") == 0) {

			is_user_added_playlist = false;
		}

		else
			is_user_added_playlist = true;

		node = next_node = current_list->head;

		for (int count2 = 0; count2 < current_list->songnumber; count2++) {
			node = next_node;

			if (count2 != current_list->songnumber - 1)
				next_node = node->next;

			data = node->data;

			if (!is_user_added_playlist) {
				delete [] data->name;
				delete [] data->singer;
				delete [] data->style;
				delete data;
			}

			delete node;
		}

	delete [] current_list->name;
	delete current_list;
	}

	node = next_node = first;
	for (int count = 0; count < current_list->songnumber; count++) {
		node = next_node;

		if (count != current_list->songnumber - 1)
			next_node = node->next;

		data = node->data;

		delete [] data->name;
		delete [] data->singer;
		delete [] data->style;
		delete data;

		delete node;
	}

	cout << endl << "Goodbye!";
}
