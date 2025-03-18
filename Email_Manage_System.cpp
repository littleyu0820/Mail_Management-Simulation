#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <set>


//設計一個郵件管理系統
/*
* 1. Folder:用來儲存郵件
* 2. 一個郵件可以儲存在多個folder內
* 3. 可以單獨在某個folder刪除郵件，但不影響整體
* 4. 可以直接刪除郵件(從全部的folder的)
*/



class Folder
{
public:

	friend class Message;

	Folder(const std::string& name) :
		folder_name(new std::string(name))
	{
		std::cout << "Folder " << name << " is created." << std::endl;
		std::cout << "-----------------------------" << std::endl;
	} //Constructor


	void Add_Msg(Message& msg); //adding mail into folder
	void Remove_From_Folder(Message& msg); //remove mail from folder
	void show_folder(); //show the details of the folder

private:

	std::string* folder_name;
	std::set<Message*> mail; //use to store the pointer which points to the mail (save mail)

};

class Message
{
public:

	friend class Folder;

	Message(const std::string& mail_name, const std::string& mail_contents):
		name(new std::string(mail_name)), content(new std::string(mail_contents))
	{
		std::cout << "Mail: " << mail_name << " is created." << std::endl;
		std::cout << "-----------------------------" << std::endl;
	} //Constructor

	Message(const Message&); //copy
	Message& operator=(const Message&); //assign

	~Message() 
	{ 
		if (!*(this->removedornot))
		{
			std::cout << *(this->name) << " is destructing..." << std::endl;
			for (auto folder : in_folder)
			{
				folder->mail.erase(this);
			}
			delete name, content;
			in_folder.clear();
		}
		else
			;
	}; //Destructor

	void Remove_Message(); //Remove the mail from all folders
	void show_mail(); //show the details of the mail


private:

	std::string* name;
	std::string* content;
	std::set<Folder*> in_folder; //store the pointer which points to the folder that stroes this mail
	bool* removedornot = new bool(false); //used to check the mail is removed or not
};

Message::Message(const Message& mail): //copy
	name(new std::string(*(mail.name))), content(new std::string(*(mail.content))){}

Message& Message::operator=(const Message& rhs) //assign
{
	auto name_temp = new std::string(*(rhs.name));
	auto content_temp = new std::string(*(rhs.content));
	delete name, content;
	name = name_temp;
	content = content_temp;
	in_folder = rhs.in_folder;

	return *this;
}

inline void Message::Remove_Message()
{
	std::cout << "Removing mail... " << *(this->name) << " from: " << std::flush;
	
	std::size_t ctr = 1; //used to check run throw the all folders or not

	for (auto folder : in_folder) //run throw the all folders that store this mail and use them to remove this mail
	{
		std::cout << *(folder->folder_name) << ((ctr < in_folder.size()) ? ", " : ".") << std::flush;
		folder->mail.erase(this);
		++ctr;
	}
	delete name, content;
	in_folder.clear();
	*(this->removedornot) = true;
	
}

inline void Message::show_mail()
{
	std::cout << "Mail: " << *(this->name) << std::endl;
	std::cout << "Contents: " << *(this->content) << std::endl;
	std::cout << "-----------------------------" << std::endl;

	auto beg = (this->in_folder).begin();
	while (beg != (this->in_folder).end())
	{
		std::cout << "Mail is stored in folder: " << *((*beg)->folder_name) << std::endl;
		std::cout << "-----------------------------" << std::endl;
		++beg;
	}

}



inline void Folder::Add_Msg(Message& msg)
{
	mail.insert(&msg);
	msg.in_folder.insert(this);
}
inline void Folder::Remove_From_Folder(Message& msg)
{
	mail.erase(&msg);
	msg.in_folder.erase(this);
}

inline void Folder::show_folder()
{
	std::cout << "Folder: " << *(this->folder_name) << std::endl;
	std::cout << "The mail in this folder: " << std::endl;

	auto beg = (this->mail).begin();
	unsigned ctr = 1;
	while (beg != (this->mail).end())
	{
		std::cout << ctr++ << ": " << *((*beg)->name) << std::endl;
		++beg;
	}

}

int main()
{

	//Create 2 folders
	Folder folder1("first_folder");
	Folder folder2("second_folder");

	//Create 3 mails
	Message msg1("First mail", "Hello, how are you?");
	Message msg2("Second mail", "Test");
	Message msg3("Third mail", "Hi");


	//adding mails to folders
	folder1.Add_Msg(msg1);
	folder1.Add_Msg(msg2);
	folder1.Add_Msg(msg3);

	folder2.Add_Msg(msg1);
	folder2.Add_Msg(msg2);

	//check details
	msg1.show_mail();
	msg2.show_mail();
	msg3.show_mail();
	folder1.show_folder();

	std::cout << std::endl;
	folder2.show_folder();
	std::cout << std::endl;


	//Remove mail from folder
	folder1.Remove_From_Folder(msg2);
	folder1.show_folder();
	std::cout << std::endl;
	msg2.show_mail();
	std::cout << std::endl;
	std::cout << std::endl;

	//Remove the whole mail
	msg1.Remove_Message();
	std::cout << std::endl;
	folder1.show_folder();
	std::cout << std::endl;


	return 0;
}
