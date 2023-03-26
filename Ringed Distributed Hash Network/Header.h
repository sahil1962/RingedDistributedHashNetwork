#include<math.h>
#include<iostream>
#include<vector>
#include<functional>
#include<fstream>
#include<algorithm>
#include<sstream>
using namespace std;
struct IdentifierNode;
struct AVL_Information_Node                   //AVL Tree Node
{
	int key = 0;
	int Row_No = 0;
	string path = "";

	AVL_Information_Node* Left;
	AVL_Information_Node* Right;
	int Height = 0;
};
/*                         AVL class    
  Purpose: it is used to store All data entered by the user and and contain address of all data
  */
class AVL_Information
{
public:
	int counting = 0;                     //for counting purpose to trace how many records are stored in the file
	AVL_Information_Node* root;            //root of AVL tree
	AVL_Information()                   //constractor
	{
		root = NULL;
	}
	void makeEmpty(AVL_Information_Node* temp)                 //to delete the tree
	{
		if (temp == NULL)
			return;
		makeEmpty(temp->Left);
		makeEmpty(temp->Right);
		delete temp;
	}

	AVL_Information_Node*& Insert(int ke, string p, AVL_Information_Node*& temp, string data, string orginal_key)  //to insert the data
	{
		if (temp == NULL)
		{
			temp = new AVL_Information_Node;


			fstream file;
			file.open(p, ios::app);                   //opening file in append mode
			file << orginal_key << "  " << data << endl;
			counting++;
			file.close();

			temp->key = ke;
			temp->Row_No = counting;
			temp->path = p;
			temp->Height = 0;
			temp->Left = temp->Right = NULL;
		}
		else if (ke < temp->key)                                    //less then 
		{
			temp->Left = Insert(ke, p, temp->Left, data, orginal_key);
			if (Height(temp->Left) - Height(temp->Right) == 2)
			{
				if (ke < temp->Left->key)
				{
					temp = singly_R(temp);
				}
				else
				{
					temp = doubly_R(temp);
				}
			}
		}
		else if (ke > temp->key)                              // greater then
		{
			temp->Right = Insert(ke, p, temp->Right, data, orginal_key);
			if (Height(temp->Right) - Height(temp->Left) == 2)
			{
				if (ke > temp->Right->key)
					temp = singly_L(temp);
				else
					temp = doubly_L(temp);
			}
		}

		temp->Height = max(Height(temp->Left), Height(temp->Right)) + 1;                        //balancing Height
		return temp;
	}


	AVL_Information_Node* Minimum(AVL_Information_Node*& temp)                    //   to find min in tree
	{
		if (temp == NULL)
		{
			return NULL;
		}
		else if (temp->Left == NULL)
		{
			return temp;
		}
		else
		{
			return Minimum(temp->Left);
		}
	}

	AVL_Information_Node* Maximum(AVL_Information_Node*& temp)                //  to find max in tree
	{
		if (temp == NULL)
		{
			return NULL;
		}
		else if (temp->Right == NULL)
		{
			return temp;
		}
		else
		{
			return Maximum(temp->Right);
		}
	}


	AVL_Information_Node* Delete(int key, AVL_Information_Node*& temp, string orginal_key)              //to delete record
	{
		
		AVL_Information_Node* current;
		if (temp == NULL)
		{
			cout << "Given input \"" << key << "\" is not found" << endl << endl;
			return NULL;
		}
		else if (key < temp->key)
		{
			temp->Left = Delete(key, temp->Left, orginal_key);
		}
		else if (key > temp->key)
		{
			temp->Right = Delete(key, temp->Right, orginal_key);
		}
		else if (temp->Left && temp->Right)
		{
			current = Minimum(temp->Right);
			temp->key = current->key;
			temp->Right = Delete(temp->key, temp->Right, orginal_key);
		}
		else
		{
			current = temp;

			if (temp->Left == NULL)
			{
				temp = temp->Right;
			}
			else if (temp->Right == NULL)
			{
				temp = temp->Left;
			}
			string reading1, reading2;
			int cou = -1;
			fstream file;
			fstream file1("tempray.txt", ios::app);                       
			file.open(current->path, ios::in);

			while (!file.eof())                  //copying all data in tempary file except which we want to delete
			{                   
				string str1, str2;
				file >> str1;
				file >> str2;
				cout << str1 << "           " << str2 << endl;
				if (str1 == orginal_key)
				{

				}
				else
				{
					file1 << str1 << "  " << str2 << endl;
					cou++;
				}
			}
			

			file.close();
			file1.close();

			ofstream file3;
			ifstream file2("tempray.txt");
			file3.open(current->path, ios::out | ios::trunc);
			file3.close();
			file3.open(current->path, ios::out | ios::app);
			string str1 = "", str2 = "";
			while (cou > 0) {                  //again restoring all data in original file

				file2 >> str1;
				file2 >> str2;
				file3 << str1 << "  " << str2 << endl;
				cou--;
			}
			file2.close();
			file3.close();

			file3.open("tempray.txt", ios::out | ios::trunc);
			file3.close();




			delete current;
		}
		if (temp == NULL)
		{
			return temp;
		}

		temp->Height = max(Height(temp->Left), Height(temp->Right)) + 1;

		if (Height(temp->Left) - Height(temp->Right) == -2)
		{
			if (Height(temp->Right->Right) - Height(temp->Right->Left) == 1)
				return singly_L(temp);

			else
				return doubly_L(temp);
		}

		else if (Height(temp->Right) - Height(temp->Left) == 2)
		{

			if (Height(temp->Left->Left) - Height(temp->Left->Right) == 1) {
				return singly_R(temp);
			}

			else
				return doubly_R(temp);
		}
		return temp;
	}
	AVL_Information_Node*& singly_R(AVL_Information_Node*& temp)                          //single Right shift
	{
		AVL_Information_Node* current = temp->Left;
		temp->Left = current->Right;
		current->Right = temp;
		temp->Height = max(Height(temp->Left), Height(temp->Right)) + 1;
		current->Height = max(Height(current->Left), temp->Height) + 1;
		return current;
	}

	AVL_Information_Node*& singly_L(AVL_Information_Node*& temp)                      //single Left  shift
	{
		AVL_Information_Node* current = temp->Right;
		temp->Right = current->Left;
		current->Left = temp;
		temp->Height = max(Height(temp->Left), Height(temp->Right)) + 1;
		current->Height = max(Height(temp->Right), temp->Height) + 1;
		return current;
	}

	AVL_Information_Node*& doubly_L(AVL_Information_Node*& temp)                    //double Left shift
	{
		temp->Right = singly_R(temp->Right);
		return singly_L(temp);
	}

	AVL_Information_Node*& doubly_R(AVL_Information_Node*& temp)                 //double Right shift
	{
		temp->Left = singly_L(temp->Left);
		return singly_R(temp);
	}
	int Height(AVL_Information_Node*& temp)                         //to get hieght of tree
	{
		if (temp == NULL)
		{
			return -1;
		}
		else
			temp->Height;
	}

	void searching_Data(int ke, AVL_Information_Node*& temp, string orginal_key, bool flag)                  //to search data using Key
	{
		if (temp == NULL)
		{
			return;
		}
		searching_Data(ke, temp->Left, orginal_key, flag);
		if (ke == temp->key)
		{
			string reading1, reading2;
			fstream file;
			file.open(temp->path);

			for (int i = 1; i <= temp->Row_No; i++)
			{
				file >> reading1;
				file >> reading2;
				if (orginal_key == reading1)
				{
					cout << "Key is: " << reading1 << "           Coressponding Data: " << reading2 << endl;
					flag = false;
					file.close();


				}
			}



		}
		if (flag)
			cout << "Record is Not Found!" << endl;
		searching_Data(ke, temp->Right, orginal_key, flag);
		return;
	}
	void reading_AVL(AVL_Information_Node*& temp, IdentifierNode* temp1, int count);                   //reading All record stored on AVL of a Machine
	void show_AVL(AVL_Information_Node* temp)                                               //printing AVL of a Machine
	{
		if (temp == NULL)
			return;
		show_AVL(temp->Left);
		cout << "Key: " << temp->key << "   Row number: " << temp->Row_No << "   path: " << temp->path << endl;
		show_AVL(temp->Right);
	}
};

                                     //distribution Hashing Table Node
struct DHT_Node {
	int serial_no;
	int Machine_id;
	IdentifierNode* M_pointer;
	DHT_Node* next, * prev;
};

//                             DHT class
class DHT {
public:
	DHT_Node* DHT_head, * DHT_tail;
	DHT()                             // constractor
	{
		DHT_head = NULL;
		DHT_tail = NULL;
	}
	void insertion(int serial, int id, IdentifierNode*& M_node) {                 //inserting information in DHT

		DHT_Node* temp = new DHT_Node;
		temp->serial_no = serial;
		temp->Machine_id = id;
		temp->M_pointer = M_node;
		temp->next = NULL;
		if (DHT_head == NULL)
		{
			DHT_head = temp;
			DHT_tail = temp;
		}
		else
		{
			DHT_tail->next = temp;
			DHT_tail = DHT_tail->next;
		}

	}
	void Delete_DHT() {                            //Deleting DHTs
		DHT_Node* temp;
		while (DHT_head != NULL) {
			temp = DHT_head->next;
			delete DHT_head;
			DHT_head = temp;

		}
	}
	void DHT_Display();                            //Displaying DHTs

};

void DHT::DHT_Display() {

	DHT_Node* temp = DHT_head;

	while (temp != NULL) {
		cout << temp->serial_no << "           " << temp->Machine_id << endl;
		temp = temp->next;
	}


}
struct IdentifierNode {                                      
	int ID;
	AVL_Information Data_keys;
	DHT Table;
	int handling_count = 0;
	vector<int> handling_nodes;
	IdentifierNode* next;
};
/*                       Indentifier Space class
* purpose: this will sepacify all the gievn space
* all the information related to machines
* linkedlist of all the machines in the space
* 
* 
* */

class Identifierspace {
public:
	int Total_No_Machine;
	vector<int> Machines_IDs;
	int count;
	int space_bits;
	IdentifierNode* head, * tail;
	Identifierspace()
	{
		space_bits = 0;
		count = 0;
		Total_No_Machine = 0;
		head = NULL;
		tail = NULL;
	}
	void set_count(int n)                 //Totall places in space
	{
		n--;
		count = n;
	}
	void set_bit(int n)                  //Totall bits in space 
	{
		space_bits = n;
	}
	void Delete_Machine(int value, int count) {                    //delete a machine
		IdentifierNode* temp = new IdentifierNode;
		int position = 0;
		temp = head;
		for (int i = 0; i < Total_No_Machine; i++) {
			if (temp->ID == value) {
				break;
			}
			position++;
			temp = temp->next;
		}
		IdentifierNode* temp1 = head;
		for (int i = 1; i < position; i++)
		{
			temp1 = temp1->next;
		}
		if (temp != head)
		{
			temp1->next = temp->next;
			temp->Data_keys.reading_AVL(temp->Data_keys.root, temp->next, count);
			delete temp;
		}
		if (temp == head)
		{
			tail->next = head->next;
			head = head->next;
			temp->Data_keys.reading_AVL(temp->Data_keys.root, temp->next, count);
			delete temp;
		}

	}

	void Machine_DHT_Display()                             //printing DHT of a Machine
	{
		int id = 0;
		IdentifierNode* temp;
		temp = head;
		
		cout << "Enter ID of Machine you want to display DHT " << endl;
		cin >> id;
		while (1)                          //check to restrict the user for a spacific input
		{
			for (int i = 0; i < Total_No_Machine; i++)
			{
				if (id == Machines_IDs[i])
					goto outside;
			}
			cout << "Wrong input! Try again: " << endl;
			cin >> id;
		}
	outside:
		for (int i = 0; i < Total_No_Machine; i++)                    //dispalying loop
		{
			if (temp->ID == id)
			{
				
				temp->Table.DHT_Display();
			}
			temp = temp->next;
		}
	}
	void Deleting_DHTs()                      //deleting
	{
		IdentifierNode* temp;
		temp = head;
		for (int i = 0; i < Total_No_Machine; i++)
		{
			temp->Table.Delete_DHT();
			temp = temp->next;
		}
	}
	void Genratiing_DHTs()                          //Genration og DHTs of all the machines
	{

		int genrated = 0, get_id = 0;
		IdentifierNode* temp, * temp1, * temp2;
		temp1 = head;
		for (int i = 0; i < Total_No_Machine; i++)
		{
			for (int j = 1; j <= space_bits; j++)
			{
				genrated = (temp1->ID + (pow(2, j - 1)));
				genrated = genrated % (count + 1);

				temp = head;
				for (int k = 0; k < Total_No_Machine; k++)
				{

					for (int h = 0; h < temp->handling_nodes.size(); h++)
					{
						if (temp->handling_nodes[h] == genrated)
						{
							temp2 = temp;
							get_id = temp->ID;
							goto out;
						}
					}
					temp = temp->next;
				}
			out:

				temp1->Table.insertion(j, get_id, temp2);
			}
			temp1 = temp1->next;
		}

	}
	void Creation_of_Machine(double value) {                              //creation of Machines  :public function of class

		IdentifierNode* temp = new IdentifierNode;
		temp->ID = value;
		temp->next = head;
		if (head == NULL) {
			head = temp;
			tail = temp;

		}
		else {
			tail->next = temp;
			tail = tail->next;
		}
	}

	void Add_Machine(int n)                        //adding machine : public function of class
	{
		Total_No_Machine = Total_No_Machine + n;
	}
	void islistEmpty() {

		if (head == NULL) {
			cout << "list is empty" << endl;
		}
		else
			cout << "List is not empty" << endl;
	}

	void display_handling_IDs_by_Machine()                        //displaying all the space handled by a machine
	{
		IdentifierNode* temp;
		temp = head;
		for (int i = 0; i < Total_No_Machine; i++)
		{

			cout << "Machine: ";
			cout << "ID " << temp->ID << endl;

			for (int k = 0; k < temp->handling_nodes.size(); k++)
			{
				cout << temp->handling_nodes[k] << " ";
			}
			cout << endl;

			temp = temp->next;
		}
	}


};

void storing_in_AVL(string s1, IdentifierNode*& temp, string Data, string Key, int AVL_key, string orginal_key)      //gernal function to call insert function(public function of  class)
{
	string convert = to_string(temp->ID);
	s1 = s1 + convert;
	s1 = s1 + ".txt";
	temp->Data_keys.Insert(AVL_key, s1, temp->Data_keys.root, Data, orginal_key);
}

void searching_in_AVL(IdentifierNode*& temp, int AVL_key, string orginal_key)    //gernal function to call searching Data function (public)
{

	temp->Data_keys.searching_Data(AVL_key, temp->Data_keys.root, orginal_key, true);
}
void Deleting_Data(int ke, IdentifierNode*& temp, string orginal_key)         //gernal function to call delete function(public)
{
	temp->Data_keys.Delete(ke, temp->Data_keys.root, orginal_key);
}



				/*
				gernal purpose function to find a machine using spacific 
				data to help in (searching,inserting and deleting proccess

				*/
IdentifierNode*& finding_machine(IdentifierNode*& temp, Identifierspace& obj, int AVL_key)      
{

	DHT_Node* temp1, * temp2;
	temp2 = temp->Table.DHT_head;
repeat:

	temp1 = temp->Table.DHT_head;

	for (int i = 0; i < obj.space_bits - 1; i++)
	{
		if (AVL_key > temp1->Machine_id && temp1->next->Machine_id >= AVL_key)               // if machine is founded where data is to be stored
		{
			
			return temp1->M_pointer;                   //returnning required machine pointer
		}

		temp1 = temp1->next;
		temp2 = temp1;
	}
	temp = temp->Table.DHT_head->M_pointer;
	for (int i = 0; i < temp->handling_nodes.size(); i++)
	{
		if (AVL_key == temp->handling_nodes[i])
		{

			return temp;
		}
	}
	//if Machine is not founded in DHT Table/ setting last machine head in Table to check again
	goto repeat;
}
void Search_Data(string Key, Identifierspace& obj, int Machine_id)        //searching data
{
	int AVL_key = 0;
	hash<string> hashing;
	IdentifierNode* temp, * temp1;
	temp1 = obj.head;
	temp = obj.head;
repeat:;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (Machine_id == temp->ID)
		{
			AVL_key = hashing(Key);
			if (AVL_key < 0)
				AVL_key = -AVL_key;

			AVL_key = AVL_key % (obj.count + 1);

			for (int j = 0; j < temp->handling_nodes.size(); j++)
			{
				if (temp->handling_nodes[j] == AVL_key)
				{
					searching_in_AVL(temp, AVL_key, Key);
					goto outside;
				}
			}
			temp1 = temp;
		}

		temp = temp->next;

	}

	temp = finding_machine(temp1, obj, AVL_key);
	Machine_id = temp->ID;
	goto repeat;
	
outside:;
}

void Deleting(string Key, Identifierspace& obj, int Machine_id)            //gernal function
{
	int AVL_key = 0;
	hash<string> hashing;
	IdentifierNode* temp, * temp1;

	temp = obj.head;
repeat:;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (Machine_id == temp->ID)
		{
			AVL_key = hashing(Key);
			if (AVL_key < 0)
				AVL_key = -AVL_key;

			AVL_key = AVL_key % (obj.count + 1);

			for (int j = 0; j < temp->handling_nodes.size(); j++)
			{
				if (temp->handling_nodes[j] == AVL_key)
				{

					Deleting_Data(AVL_key, temp, Key);
					goto outside;
				}

			}
			temp1 = temp;
		}

		temp = temp->next;

	}
	temp = finding_machine(temp1, obj, AVL_key);
	Machine_id = temp->ID;
	goto repeat;
	
outside:;

}
void Store_Data(string Key, string Data, Identifierspace& obj, int Machine_id)         //gernal function storing information in AVl of machine

{

	int AVL_key = 0;
	hash<string> hashing;
	IdentifierNode* temp, * temp1;
	string s1 = "Machine", s2 = ".txt";
	temp = obj.head;
	temp1 = obj.head;
repeat:;

	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (Machine_id == temp->ID)
		{
			AVL_key = hashing(Key);
			if (AVL_key < 0)
				AVL_key = -AVL_key;

			AVL_key = AVL_key % (obj.count + 1);
			cout << "hashing " << AVL_key << endl;
			for (int j = 0; j < temp->handling_nodes.size(); j++)                            //e==p
			{
				if (temp->handling_nodes[j] == AVL_key)
				{
					storing_in_AVL(s1, temp, Data, Key, AVL_key, Key);
					goto outside;
				}
			}
			temp1 = temp;
			if (temp->ID < AVL_key && AVL_key <= temp->Table.DHT_head->Machine_id)
			{
				temp = temp->Table.DHT_head->M_pointer;
				for (int j = 0; j < temp->handling_nodes.size(); j++)                            //e>last machine ID
				{
					if (temp->handling_nodes[j] == AVL_key)
					{
						storing_in_AVL(s1, temp, Data, Key, AVL_key, Key);
						goto outside;
					}
				}
			}
		}

		temp = temp->next;

	}

	

	temp = finding_machine(temp1, obj, AVL_key);
	Machine_id = temp->ID;
	goto repeat;

outside:;
}

						/*
						 *  spacifying  total space
						*seting space variable
						* seting bits in space
						*/

void insert(int n, Identifierspace& obj)//helping the insertion in identifier space.                    
{
	int p;
	p = pow(2, n);
	obj.set_bit(n);
	obj.set_count(p);

}
void Delete_Machine_on_Fly(Identifierspace& obj)                       //deleting machine on run time
{
	int input, match1 = 0;
	IdentifierNode* temp, * temp1;
	cout << "Enter ID of Machine which you want to Delete: " << endl;
	cin >> input;
	while (1)
	{
		for (int i = 0; i < obj.Total_No_Machine; i++)
		{
			if (input == obj.Machines_IDs[i])
				goto outside;
		}
		cout << "Wrong input! Try again: " << endl;
		cin >> input;
	}
outside:
	temp1 = obj.head;
	temp = obj.head;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (obj.Machines_IDs[i] == input && (obj.Machines_IDs[obj.Machines_IDs.size() - 1]) != input)
		{
			match1 = obj.Machines_IDs[i + 1];
			remove(obj.Machines_IDs.begin(), obj.Machines_IDs.end(), input);
		}
		else if (obj.Machines_IDs[obj.Machines_IDs.size() - 1] == input)
		{
			match1 = obj.Machines_IDs[0];
			remove(obj.Machines_IDs.begin(), obj.Machines_IDs.end(), 0);
		}
	}

	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (temp->ID == input)
		{
			for (int i = 0; i < obj.Total_No_Machine; i++)
			{
				if (temp1->ID == match1)
				{
					for (int k = (temp->handling_nodes.size() - 1); k >= 0; k--)
					{
						temp1->handling_nodes.insert(temp1->handling_nodes.begin(), temp->handling_nodes[k]);

					}
					obj.Deleting_DHTs();
					obj.Delete_Machine(input, obj.count);
					obj.Total_No_Machine--;
					obj.Genratiing_DHTs();
					goto outside1;
				}
				temp1 = temp1->next;
			}
		}
		temp = temp->next;
	}
outside1:;
}
void Add_Machine_on_Fly(Identifierspace& obj)                 //adding machine on run time
{
	obj.Deleting_DHTs();
	int input, input2;
	IdentifierNode* temp, * temp1, * temp2;
	vector<int> var1;
	cout << "Please Enter ID of New Machine: " << endl;
	cin >> input;
again:
	for (int i = 0; i < obj.Machines_IDs.size(); i++)
	{
		if (obj.Machines_IDs[i] == input)
		{
			cout << "this ID is Already Registered! Please Try Again;- " << endl;
			cin >> input;
			goto again;
			
		}

	}
	
	

	temp = obj.head;
	temp2 = obj.tail;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		for (int j = 0; j < temp->handling_nodes.size(); j++)
		{
			if (temp->handling_nodes[j] == input)
			{
				obj.Creation_of_Machine(input);
				obj.Machines_IDs.push_back(input);
				for (int i = 0; i < obj.Machines_IDs.size(); i++)
				{
					sort(obj.Machines_IDs.begin(), obj.Machines_IDs.end());
				}

				obj.Add_Machine(1);
				temp1 = obj.head;
				for (int k = 0; k < obj.Total_No_Machine; k++)
				{
					if (temp1->ID == input)
					{
						for (int l = 0; l < temp->handling_nodes.size(); l++)
						{
							if (temp->handling_nodes[l] <= input)
								temp1->handling_nodes.push_back(temp->handling_nodes[l]);
							else if (temp->handling_nodes[l] > input)
								temp1->handling_nodes.push_back(temp->handling_nodes[l]);
							if (temp->handling_nodes[l] == input)
							{
								input2 = l;
								input2++;
								goto stop;
							}
							//	var1.push_back(temp->handling_nodes[l]);
						}
					stop:
						for (input2; input2 < temp->handling_nodes.size(); input2++)
						{
							var1.push_back(temp->handling_nodes[input2]);
						}
						temp->handling_nodes.clear();
						temp->handling_nodes = var1;
						goto outside;
					}
					temp2 = temp1;
					temp1 = temp1->next;
				}
			}

		}


		temp = temp->next;
	}
	obj.Genratiing_DHTs();
outside:;
}


void Creat_Machines(int n, Identifierspace& obj)                       //create machine manually   at start of program
{
	vector<int> index;    //vector to store indexes which user enter     
	IdentifierNode* temp, * temp1;
	int input, input1 = 0, var1 = 0;
	string Name = "";

	for (int i = 0; i < n; i++)                // taking input from user
	{

		cout << "Enter ID number where you want to create a Machine " << i + 1 << endl;
		cin >> input;
		while (input<0 || input>obj.count + 1)
		{
			cout << "Wrong input ID Please Try Again! " << endl;
			cin >> input;
		}
	again:
		for (int i = 0; i < index.size(); i++)
		{
			if (index[i] == input)
			{
				cout << "this ID is Already registered! Please Try Again;- " << endl;
				cin >> input;
				goto again;

			}

		}
		index.push_back(input);
	}

	for (int i = 0; i < index.size(); i++)           //sorting
	{
		sort(index.begin(), index.end());
	}
	obj.Machines_IDs = index;
	for (int i = 0; i < n; i++)              //creating machine
	{
		obj.Creation_of_Machine(index[i]);
	}
	temp = obj.head;
	if (obj.Total_No_Machine > 1)
	{
		for (int i = 0; i < n; i++)
		{
			temp1 = obj.head;

			input = temp->ID;
			for (int j = 0; j < obj.Total_No_Machine; j++)
			{
				if (temp1->next->ID == input)
				{
					input1 = temp1->ID;                   //ID of previous machine
				}
				temp1 = temp1->next;
			}
			if (input1 == obj.count)
			{
				input1 = 0;
				for (int k = input1; k <= input; k++)
				{

					temp->handling_nodes.push_back(k);
				}
			}
			else
			{
				input1++;
				for (int k = input1; k <= input; k++)
				{

					temp->handling_nodes.push_back(k);

					if (k == 0)
						k = obj.count;
					var1 = k;
				}
			}

			temp = temp->next;
		}
		var1++;
		//	cout << "var1 " << var1 << endl;
		temp = obj.head;
		for (int i = var1; i <= obj.count; i++)
		{
			if (temp->ID + 1 == i)
				break;
			temp->handling_nodes.push_back(i);

			if (i == obj.count)
				i = -1;
		}

	}
	else
	{
		temp = obj.head;
		for (int i = 0; i <= obj.count; i++)
		{

			temp->handling_nodes.push_back(i);

		}
	}
	
	temp = obj.head;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		cout << temp->ID << endl;
		temp = temp->next;
	}

	obj.Genratiing_DHTs();

}

void Creat_Machines_Automatically(int n, Identifierspace& obj)                   //create machine Automatically   at start of program
{

	string input_Name;
	int genrate;
	vector<int> index;
	IdentifierNode* temp, * temp1;
	int input, input1 = 0, var1 = 0;
	for (int i = 0; i < n; i++)
	{
		cout << "Enter Name of  Machine " << i + 1 << endl;
		cin >> input_Name;
		hash<string> hashing;
		genrate = hashing(input_Name) % obj.count + 1;
		for (int j = 0; j < index.size(); j++)
		{
			if (genrate == index[j])
			{
				genrate++;
				j = 0;
			}
		}
		index.push_back(genrate);
	}
	for (int i = 0; i < index.size(); i++)
	{
		sort(index.begin(), index.end());
	}
	obj.Machines_IDs = index;
	for (int i = 0; i < n; i++)
	{
		obj.Creation_of_Machine(index[i]);
	}

	temp = obj.head;
	if (obj.Total_No_Machine > 1)
	{
		for (int i = 0; i < n; i++)
		{
			temp1 = obj.head;

			input = temp->ID;
			for (int j = 0; j < obj.Total_No_Machine; j++)
			{
				if (temp1->next->ID == input)
				{
					input1 = temp1->ID;                   //ID of previous machine
				}
				temp1 = temp1->next;
			}

			if (input1 == obj.count)
			{
				input1 = 0;
				for (int k = input1; k <= input; k++)
				{

					temp->handling_nodes.push_back(k);
				}
			}
			else
			{
				input1++;

				for (int k = input1; k <= input; k++)
				{

					temp->handling_nodes.push_back(k);

					if (k == 0)
						k = obj.count + 1;
					var1 = k;
				}
			}

			temp = temp->next;
		}
		var1++;
		
		temp = obj.head;
		for (int i = var1; i <= obj.count; i++)
		{
			if (temp->ID + 1 == i)
				break;
			temp->handling_nodes.push_back(i);
			cout << i << endl;
			if (i == obj.count)
				i = -1;
		}
	}
	else
	{
		temp = obj.head;
		for (int i = 0; i <= obj.count; i++)
		{

			temp->handling_nodes.push_back(i);

		}
	}

	obj.Genratiing_DHTs();


}
void addingMachine(int n, Identifierspace& obj)             //gernal purpose function 
{
	obj.Add_Machine(n);
}
void AVL_Information::reading_AVL(AVL_Information_Node*& temp, IdentifierNode* temp1, int count)                   //printing complete infomation of a machine from file
{

	if (temp == NULL)
	{
		return;
	}
	reading_AVL(temp->Left, temp1, count);
	fstream file;
	string str1, str2;
	hash<string> hashing;
	int genrate = 0;
	file.open(temp->path);
	while (!file.eof())
	{
		file >> str1;
		file >> str2;
		
		genrate = hashing(str1);
		genrate = genrate % (count + 1);
		if (genrate == temp->key)
			temp1->Data_keys.Insert(temp->key, temp1->Data_keys.root->path, temp1->Data_keys.root, str2, str1);
	}
	file.close();
	reading_AVL(temp->Right, temp1, count);


}
void Showing_AVL(int ID, Identifierspace& obj)              //printing AVL of a machine
{
	IdentifierNode* temp = obj.head;
	for (int i = 0; i < obj.Total_No_Machine; i++)
	{
		if (ID == temp->ID)
		{
			temp->Data_keys.show_AVL(temp->Data_keys.root);
		}

		temp = temp->next;
	}
}