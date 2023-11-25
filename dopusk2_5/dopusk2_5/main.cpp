#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <numeric>

#define e 2.718281828459045

using namespace std;

struct ALOHA_strust {
	double M_N;
	double M_D;
	double lambda_output;
	double lambda_pract;
};

struct system_strust {
	double M_N;
	double M_N_theor;
	double M_D_synchr;
	double M_D_theor_synchr;
	double M_D_asynchr;
	double M_D_theor_asynchr;
};

class SMS {
private:

	int factorial(int i) {
		if (i == 0)
			return 1;
		else
			return i * factorial(i - 1);
	}

	int poisson_generate(double lam) {
		int lst_res = 0;
		double p = (double)rand() / (double)RAND_MAX;
		double cur_sum = 0;
		for (int i = 0; i < 20; i++) {
			cur_sum += (pow(lam, i)) / (double)factorial(i) * (pow(e, (-lam)));
			if (p <= cur_sum) {
				lst_res = i;
				break;
			}
		}

		return lst_res;
	}

	vector<int> generate_flow(double lam, int size) {
		vector<int> lst_res;
		lst_res.resize(size);
		for (int k = 0; k < size; k++) {
			double p = (double)rand() / (double)RAND_MAX;
			double cur_sum = 0;
			for (int i = 0; i < 20; i++) {
				cur_sum += (pow(lam, i)) / (double)factorial(i) * (pow(e, (-lam)));
				if (p <= cur_sum) {
					lst_res[k] = i;
					break;
				}
			}
		}
		return lst_res;
	}

	int ready(double p) {
		return p >= (double)(rand()) / RAND_MAX;
	}


	int sum(vector<int>& vec) {
		int res = 0;
		for (size_t i = 0; i < vec.size(); i++) {
			res += vec[i];
		}
		return res;
	}

	double sum(vector<double>& vec) {
		double res = 0;
		for (size_t i = 0; i < vec.size(); i++) {
			res += vec[i];
		}
		return res;
	}

	int find(vector<int>& vec, int seek) {
		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] == seek)
				return i;
		}
		return -1;
	}


public:
	ALOHA_strust ALOHA(double lambda_val, int M, int N, double p) {
		/*
		:param lambda_val: интенсивность потока
		:param M: количество абонентов
		:param N: количество окон
		:param p: первоначальная вероятность передачи
		*/
		int counter1 = 0;
		int counter2 = 0;
		int counter3 = 0;
		vector<int> subscribers, ready_send, delay, success, new_message, cur_quantity_subscribers;
		vector<vector<double>> start_time;
		subscribers.resize(M);
		ready_send.resize(M);
		start_time.resize(M);
		for (size_t i = 0; i < start_time.size(); i++)
			start_time[i].push_back(-1);
		int new_messages_in_window = 0, quantity_messages = 0;
		

		for (int cur_window = 0; cur_window < N; cur_window++) {
			new_messages_in_window = 0;
			for (size_t i = 0; i < subscribers.size(); i++) {
				quantity_messages = poisson_generate(lambda_val / (double)M);
				subscribers[i] += quantity_messages;
				new_messages_in_window += quantity_messages;
				if (subscribers[i] != 0) {
					if (start_time[i][0] == -1) {
						start_time[i][0] = cur_window + (double)rand() / (double)RAND_MAX;
						for (int j = 1; j < subscribers[i]; j++) {
							start_time[i].push_back(cur_window + (double)rand() / (double)RAND_MAX);
						}
					}
					while (start_time[i].size() < subscribers[i]) {
						start_time[i].push_back(cur_window + (double)rand() / (double)RAND_MAX);
						sort(start_time[i].begin(), start_time[i].end());
					}
					ready_send[i] = ready(p);
				}
			}

			
			new_message.push_back(new_messages_in_window);
			cur_quantity_subscribers.push_back(sum(subscribers));
			

			if (sum(ready_send) != 1 && sum(ready_send) != 0) {
				for (size_t i = 0; i < ready_send.size(); i++)
					ready_send[i] = 0;
				success.push_back(0);
				counter1++;
			}
			else if (sum(ready_send) == 0) {
				success.push_back(0);
				counter2++;
			}
			else {
				int index = find(ready_send, 1);
				subscribers[index] -= 1;
				ready_send[index] = 0;
				delay.push_back(cur_window - start_time[index][0] + 2.5);
				start_time[index].erase(start_time[index].begin());
				if (start_time[index].size() == 0)
					start_time[index].push_back(-1);
				success.push_back(1);
				counter3++;
			}

		}
		
	
		ALOHA_strust str;
		str.M_N = (double)sum(cur_quantity_subscribers) / (double)cur_quantity_subscribers.size();
		str.M_D = 0;
		if (delay.size() == 0)
			str.M_D = 1.5;
		else
			str.M_D = (double)sum(delay) / (double)delay.size();
		str.lambda_output = (double)sum(success) / (double)success.size();
		str.lambda_pract = (double)sum(new_message) / (double)new_message.size();



		return str;
	}


	ALOHA_strust ALOHA_ONE_V2(double lambda_val, int M, int N, double p) {
		/*
		:param lambda_val: интенсивность потока
		:param M: количество абонентов
		:param N: количество окон
		:param p: первоначальная вероятность передачи
		*/
		vector<int> subscribers, ready_send, delay, success, new_message, cur_quantity_subscribers;
		vector<bool> first_message_flag;
		vector<vector<double>> start_time;
		subscribers.resize(M);
		ready_send.resize(M);
		start_time.resize(M);
		first_message_flag.resize(M);
		for (size_t i = 0; i < start_time.size(); i++)
			start_time[i].push_back(-1);
		int new_messages_in_window = 0, quantity_messages = 0;

		for (int cur_window = 0; cur_window < N; cur_window++) {
			new_messages_in_window = 0;
			for (size_t i = 0; i < subscribers.size(); i++) {
				quantity_messages = poisson_generate(lambda_val / (double)M);
				subscribers[i] += quantity_messages;
				new_messages_in_window += quantity_messages;
				if (subscribers[i] != 0) {
					if (start_time[i][0] == -1) {
						start_time[i][0] = cur_window + (double)rand() / (double)RAND_MAX;
						for (int j = 1; j < subscribers[i]; j++) {
							start_time[i].push_back(cur_window + (double)rand() / (double)RAND_MAX);
						}
					}
					while (start_time[i].size() < subscribers[i]) {
						start_time[i].push_back(cur_window + (double)rand() / (double)RAND_MAX);
						sort(start_time[i].begin(), start_time[i].end());
					}
					if (first_message_flag[i] == false) {
						ready_send[i] = ready(1);
						first_message_flag[i] = true;
					}
					else ready_send[i] = ready(p);
				}
			}
			new_message.push_back(new_messages_in_window);
			cur_quantity_subscribers.push_back(sum(subscribers));

			

			if (sum(ready_send) != 1 && sum(ready_send) != 0) {
				for (size_t i = 0; i < ready_send.size(); i++) {
					ready_send[i] = 0;
				}
				success.push_back(0);
			}
			else if (sum(ready_send) == 0) success.push_back(0);
			else {
				int index = find(ready_send, 1);
				subscribers[index] -= 1;
				ready_send[index] = 0;
				first_message_flag[index] = false;
				
				delay.push_back(cur_window + 1 - start_time[index][0] + 1.5);
				start_time[index].erase(start_time[index].begin());
				if (start_time[index].size() == 0)
					start_time[index].push_back(-1);
				success.push_back(1);
			}

			

		}
		ALOHA_strust str;
		str.M_N = (double)sum(cur_quantity_subscribers) / (double)cur_quantity_subscribers.size();
		str.M_D = 0;
		if (delay.size() == 0)
			str.M_D = 1.5;
		else
			str.M_D = (double)sum(delay) / (double)delay.size();
		str.lambda_output = (double)sum(success) / (double)success.size();
		str.lambda_pract = (double)sum(new_message) / (double)new_message.size();
		return str;
	}
	


	system_strust generate_system(int N, double lambda_val) {
		vector<int> number_subscribers = generate_flow(lambda_val, N);
		int time = 0, last_size = 0;
		bool isLast;
		double differ = 0;
		vector<double> delay_synchr;
		vector<double> delay_asynchr;
		vector<int> cur_quantity_subscribers;
		vector<pair<int, double>> queue_all;
		for (int cur_N = 0; cur_N < N; cur_N++) {
			if (last_size == 0)
				isLast = true;
			else
				isLast = false;
			last_size = queue_all.size();
			for (int subscriber = 0; subscriber < number_subscribers[cur_N]; subscriber++)
				queue_all.push_back(make_pair(subscriber, (double)time + (double)rand() / (double)RAND_MAX));
			sort(queue_all.begin(), queue_all.end(), [](auto& left, auto& right) { return left.second < right.second; });
			if (queue_all.size() != 0) {
				if (queue_all[0].second < (double)time) {
					delay_synchr.push_back(time - queue_all[0].second + 1.0);
					if (isLast) {
						delay_asynchr.push_back(1);
						differ = queue_all[0].second + 1.0;
					}
					else if (differ > queue_all[0].second) {
						delay_asynchr.push_back(differ - queue_all[0].second + 1.0);
						differ = differ + 1.0;
					}
					else {
						delay_asynchr.push_back(1.0);
						differ = queue_all[0].second + 1;
					}
					queue_all.erase(queue_all.begin());
				}
			}
			cur_quantity_subscribers.push_back(queue_all.size());
			time += 1;
		}

		if (delay_synchr.size() == 0) delay_synchr.push_back(1.5);
		if (delay_asynchr.size() == 0) delay_asynchr.push_back(1);
		system_strust str;
		str.M_D_synchr = (double)sum(delay_synchr) / (double)delay_synchr.size();
		str.M_N = (double)sum(cur_quantity_subscribers) / (double)cur_quantity_subscribers.size();
		str.M_N_theor = (lambda_val * (2.0 - lambda_val)) / (2.0 * (1.0 - lambda_val));
		str.M_D_theor_synchr = ((2.0 - lambda_val) / (2.0 * (1.0 - lambda_val))) + 0.5;
		str.M_D_asynchr = (double)sum(delay_asynchr) / (double)delay_asynchr.size();
		str.M_D_theor_asynchr = ((2.0 - lambda_val) / (2.0 * (1.0 - lambda_val)));
		return str;
	}

	void write(vector<ALOHA_strust> mas_ALOHA, vector<system_strust> system) {

		ofstream out_ALOHA_M_N;
		ofstream out_ALOHA_M_D;
		ofstream out_ALOHA_lambda_output;
		ofstream out_ALOHA_lambda_pract;
	
		ofstream out_SYNCHR_M_N_sync;
		ofstream out_SYNCHR_M_N_async;
		ofstream out_SYNCHR_M_N_theor_all;
		ofstream out_SYNCHR_M_D_synchr;
		ofstream out_SYNCHR_M_D_theor_synchr;
		ofstream out_SYNCHR_M_D_asynchr;
		ofstream out_SYNCHR_M_D_theor_asynchr;

		out_ALOHA_M_N.open("ALOHA_M_N_BACK.dat");
		out_ALOHA_M_D.open("ALOHA_M_D_BACK.dat");
		out_ALOHA_lambda_output.open("ALOHA_lambda_output.dat");
		out_ALOHA_lambda_pract.open("ALOHA_lambda_pract.dat");

		out_SYNCHR_M_N_sync.open("SYNCHR_M_N_sync.dat");
		out_SYNCHR_M_N_async.open("SYNCHR_M_N_async.dat");
		out_SYNCHR_M_N_theor_all.open("SYNCHR_M_N_theor_all.dat");
		out_SYNCHR_M_D_synchr.open("SYNCHR_M_D_synchr.dat");
		out_SYNCHR_M_D_theor_synchr.open("SYNCHR_M_D_theor_synchr.dat");
		out_SYNCHR_M_D_asynchr.open("SYNCHR_M_D_asynchr.dat");
		out_SYNCHR_M_D_theor_asynchr.open("SYNCHR_M_D_theor_asynchr.dat");

		for (int lambda_val = 0; lambda_val < 100; lambda_val++) {

			out_ALOHA_M_N << mas_ALOHA[lambda_val].M_N << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_M_D << mas_ALOHA[lambda_val].M_D << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_lambda_output << mas_ALOHA[lambda_val].lambda_output << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_lambda_pract << mas_ALOHA[lambda_val].lambda_pract << " " << (double)lambda_val / 100 << "\n";

		//	out_SYNCHR_M_N_sync << system[lambda_val].M_N << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_N_async << system[lambda_val].M_N << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_N_theor_all << system[lambda_val].M_N_theor << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_D_synchr << system[lambda_val].M_D_synchr << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_D_theor_synchr << system[lambda_val].M_D_theor_synchr << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_D_asynchr << system[lambda_val].M_D_asynchr << " " << (double)lambda_val / 100 << "\n";
		//	out_SYNCHR_M_D_theor_asynchr << system[lambda_val].M_D_theor_asynchr << " " << (double)lambda_val / 100 << "\n";
		}

		out_SYNCHR_M_N_sync.close();
		out_SYNCHR_M_N_async.close();
		out_SYNCHR_M_N_theor_all.close();
		out_SYNCHR_M_D_synchr.close();
		out_SYNCHR_M_D_theor_synchr.close();
		out_SYNCHR_M_D_asynchr.close();
		out_SYNCHR_M_D_theor_asynchr.close();
	}

	void write_for_one(vector<ALOHA_strust> mas_ALOHA, vector<system_strust> system) {

		ofstream out_ALOHA_M_N;
		ofstream out_ALOHA_M_D;
		ofstream out_ALOHA_lambda_output;
		ofstream out_ALOHA_lambda_pract;

		ofstream out_SYNCHR_M_N_sync;
		ofstream out_SYNCHR_M_N_async;
		ofstream out_SYNCHR_M_N_theor_all;
		ofstream out_SYNCHR_M_D_synchr;
		ofstream out_SYNCHR_M_D_theor_synchr;
		ofstream out_SYNCHR_M_D_asynchr;
		ofstream out_SYNCHR_M_D_theor_asynchr;

		out_ALOHA_M_N.open("ALOHA_M_N_1_BACK.dat");
		out_ALOHA_M_D.open("ALOHA_M_D_1_BACK.dat");
		out_ALOHA_lambda_output.open("ALOHA_lambda_output_1.dat");
		out_ALOHA_lambda_pract.open("ALOHA_lambda_pract_1.dat");


		for (int lambda_val = 0; lambda_val < 100; lambda_val++) {

			out_ALOHA_M_N << mas_ALOHA[lambda_val].M_N << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_M_D << mas_ALOHA[lambda_val].M_D << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_lambda_output << mas_ALOHA[lambda_val].lambda_output << " " << (double)lambda_val / 100 << "\n";
			out_ALOHA_lambda_pract << mas_ALOHA[lambda_val].lambda_pract << " " << (double)lambda_val / 100 << "\n";

		}
	}


	void write_p(vector<ALOHA_strust> mas_ALOHA, double lambda, double p_array[]) {

		ofstream out_ALOHA_M_N;
		ofstream out_ALOHA_M_D;
		ofstream out_ALOHA_lambda_output;
		ofstream out_ALOHA_lambda_pract;

		out_ALOHA_M_N.open("ALOHA_M_N.dat");
		out_ALOHA_M_D.open("ALOHA_M_D.dat");
	

		for (int i = 0; i < 7; i++) {

			out_ALOHA_M_N << mas_ALOHA[i].M_N << " " << p_array[i] << "\n";
			out_ALOHA_M_D << mas_ALOHA[i].M_D << " " << p_array[i] << "\n";

		}


		out_ALOHA_M_N.close();
		out_ALOHA_M_D.close();


	}


	void write_p_one(vector<ALOHA_strust> mas_ALOHA, double lambda, double p_array[]) {

		ofstream out_ALOHA_M_N;
		ofstream out_ALOHA_M_D;
		ofstream out_ALOHA_lambda_output;
		ofstream out_ALOHA_lambda_pract;

		out_ALOHA_M_N.open("ALOHA_M_N_1.dat");
		out_ALOHA_M_D.open("ALOHA_M_D_1.dat");


		for (int i = 0; i < 7; i++) {

			out_ALOHA_M_N << mas_ALOHA[i].M_N << " " << p_array[i] << "\n";
			out_ALOHA_M_D << mas_ALOHA[i].M_D << " " << p_array[i] << "\n";

		}


		out_ALOHA_M_N.close();
		out_ALOHA_M_D.close();


	}



};


int main() {
	srand((unsigned int)time(0));


	
	int N = 50000;
	int M = 10;
	double p = (double)1/M;
	SMS a;
	vector<system_strust> system;
	vector<ALOHA_strust> mas_ALOHA;

	vector<ALOHA_strust> example;
	vector<ALOHA_strust> example2;
	for (int lambda_val = 0; lambda_val < 100; lambda_val++) {
		mas_ALOHA.push_back(a.ALOHA((double)lambda_val / (double)100, M, N, p));
		example.push_back(a.ALOHA_ONE_V2((double)lambda_val / (double)100, M, N, p));
		//system.push_back(a.generate_system(N, (double)lambda_val / (double)100));
	}
	a.write(mas_ALOHA,system);
	a.write_for_one(example, system);
	

	
	/*
	int N = 100000;
	int M = 10;

	double p_array[] = { 0.025, 0.05, 0.075 , 0.1, 0.125, 0.150, 0.175 };
	SMS a;
	vector<ALOHA_strust> ALOHA;
	vector<ALOHA_strust> ALOHA_ONE;

	double lambda = 0.25;

	for (int i = 0; i < 7; i++) {
		ALOHA_ONE.push_back(a.ALOHA_ONE_V2(lambda, M, N, p_array[i]));
		ALOHA.push_back(a.ALOHA(lambda, M, N, p_array[i]));
		
	}

	a.write_p(ALOHA, lambda, p_array);
	a.write_p_one(ALOHA_ONE, lambda, p_array);
	*/
	

	return 0;
} 