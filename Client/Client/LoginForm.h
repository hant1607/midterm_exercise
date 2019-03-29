#pragma comment(lib,"ws2_32.lib")
#include<cstdlib>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<string>
#include "ServerInforForm.h"
#include "MainChatInterface.h"
namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for LoginForm
	/// </summary>
	public ref class LoginForm : public System::Windows::Forms::Form
	{
	public:
		LoginForm(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LoginForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  choise;
	private: Client::MainChatInterface main;
	private: Thread^ t;
	private: SOCKET connection;
	private: void initSocket(std::string ip, int port) {
		WSAData wsaData;
		WORD DllVersion = MAKEWORD(2, 1);
		if (WSAStartup(DllVersion, &wsaData) != 0) {
			MessageBox::Show("Winsock startup failed", "Error", MessageBoxButtons::OK);
			return;
		}

		sockaddr_in addr;
		int addrlen = sizeof(addr);
		inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;

		connection = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			MessageBox::Show("Failed to connect", "Error", MessageBoxButtons::OK);
			return;
		}
	}
	private: void createHandle() {
		char data[1024];
		while (true) {
			ZeroMemory(data, sizeof(data));
			strcpy_s(data, "Hello server!");
			send(connection, data, sizeof(data), 0);
			ZeroMemory(data, sizeof(data));
			recv(connection, data, sizeof(data), 0);
			main.displayMessage(gcnew System::String(data));
			main.ShowDialog();
		}
		t->Abort();
	}
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->choise = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(37, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"1. Create room";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(37, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(64, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"2. Join room";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(37, 70);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(36, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"3. Exit";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(41, 99);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(95, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Enter your choise: ";
			// 
			// choise
			// 
			this->choise->Location = System::Drawing::Point(154, 96);
			this->choise->Name = L"choise";
			this->choise->Size = System::Drawing::Size(88, 20);
			this->choise->TabIndex = 4;
			this->choise->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::choise_keyPressed);
			// 
			// LoginForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(300, 150);
			this->Controls->Add(this->choise);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"Login Form";
			this->Text = L"LoginForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void choise_keyPressed(System::Object^  sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == Keys::Enter) {
			Int64 choiseTemp = System::Convert::ToInt64(choise->Text);
			if (choiseTemp <= 0 || choiseTemp > 3) {
				MessageBox::Show("Lua chon khong hop le!", "Error", MessageBoxButtons::OK);
			}
			if (choiseTemp.CompareTo(3) == 0) {
				LoginForm::Close();
			}
			if (choiseTemp.CompareTo(2) == 0) {
				this->Hide();
				initSocket("127.0.0.1", 1080);
				t = gcnew Thread(gcnew ThreadStart(this, &LoginForm::createHandle));
				t->Start();
			}
			if (choiseTemp.CompareTo(1) == 0) {
				this->Hide();
				Client::ServerInforForm serverInfor;
				serverInfor.ShowDialog();
				this->Show();
			}
			choise->Text = "";
		}
	}
};
}
