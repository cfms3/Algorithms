namespace FFT {
	typedef long long int ll;
	typedef complex<double> cd;
	
	const double PI = acos(-1.0);
	
	void fft(vector<cd> &a, bool inv) {
		int n = (int) a.size();
		if (n == 1) return;
		
		for(int i = 0, j = 0; i < n; i++) {
			if (i > j) {
				swap(a[i], a[j]);
			}
			for(int l = n / 2; (j ^= l) < l; l >>= 1);
		}
		
		double ang = inv ? -PI : PI;
		for(int sz = 1; sz < n; sz *= 2) {
			cd delta(cos(ang / sz), sin(ang / sz));
			for(int i = 0; i < n; i += 2 * sz) {
				cd cur_root(1, 0);
				for(int j = 0; j < sz; j++) {
					cd u = a[i + j], v = cur_root * a[i + j + sz];
					a[i + j] = u + v;
					a[i + j + sz] = u - v;
					cur_root *= delta;
				}
			}
		}
		if (inv) {
			for(int i = 0; i < n; i++) a[i] /= n;
		}
	}
	
	void multiply(vector<ll> &a, vector<ll> &b, vector<ll> &ans) {
		int n = 1;
		while(n < (int) max(a.size(), b.size())) n <<= 1;
		n <<= 1;
		vector<cd> t_a(n, cd(0)), t_b(n, cd(0));
		for(int i = 0; i < (int) a.size(); i++) {
			t_a[i] = cd(a[i]);
		}
		for(int i = 0; i < (int) b.size(); i++) {
			t_b[i] = cd(b[i]);
		}
		//fft
		fft(t_a, false);
		fft(t_b, false);
		//convolution
		for(int i = 0; i < n; i++) {
			t_a[i] = t_a[i] * t_b[i];
		}
		//inverse fft
		fft(t_a, true);
		ans.resize(n);
		for(int i = 0; i < n; i++) {
			ans[i] = ll(t_a[i].real() + 0.5);
		}
	} 
};

namespace FFT {
	typedef long long int ll;
	typedef complex<double> cd;
	
	const double PI = acos(-1.0);
	
	void fft(vector<cd> &a, bool inv) {
		int n = (int) a.size();
		if (n == 1) return;
		vector<cd> even(n >> 1), odd(n >> 1);
		for(int i = 0, j = 0; i < n; i += 2, j++) {
			even[j] = a[i];
			odd[j] = a[i + 1];
		}
		fft(even, inv);
		fft(odd, inv);
		double ang = 2.0 * PI / double(n);
		if (inv) ang *= -1.0;
		cd cur_root(1);
		cd delta(cos(ang), sin(ang));
		for(int i = 0; i < n / 2; i++) {
			cd v = even[i], u = cur_root * odd[i];
			a[i] = v + u;
			a[i + n / 2] = v - u;
			if (inv) {
				a[i] /= 2;
				a[i + n / 2] /= 2;
			}
			cur_root *= delta;
		}
	}
	
	void multiply(vector<ll> &a, vector<ll> &b, vector<ll> &ans) {
		int n = 1;
		while(n < (int) max(a.size(), b.size())) n <<= 1;
		n <<= 1;
		vector<cd> t_a(n, cd(0)), t_b(n, cd(0));
		for(int i = 0; i < (int) a.size(); i++) {
			t_a[i] = cd(a[i]);
		}
		for(int i = 0; i < (int) b.size(); i++) {
			t_b[i] = cd(b[i]);
		}
		//fft
		fft(t_a, false);
		fft(t_b, false);
		//convolution
		for(int i = 0; i < n; i++) {
			t_a[i] = t_a[i] * t_b[i];
		}
		//inverse fft
		fft(t_a, true);
		ans.resize(n);
		for(int i = 0; i < n; i++) {
			ans[i] = ll(t_a[i].real() + 0.5);
		}
	} 
};