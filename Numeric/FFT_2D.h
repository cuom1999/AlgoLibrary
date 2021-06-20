typedef complex<double> base;
	
namespace FFT_2D {
	vector<int> inv_fft;
	vector<base> roots;
		
	void preCalcFFT(int n, bool inv) {
		inv_fft.resize(n);

		int j = 0;
		for(int i=1; i<n; i++)
		{
			int bit = (n >> 1);
			while(j >= bit)
			{
				j -= bit;
				bit >>= 1;
			}
			j += bit;
			inv_fft[i] = j;
		}
		roots.resize(n / 2);
		double ang = 2 * acos(-1) / n * (inv ? -1 : 1);
		
		for(int i=0; i<n/2; i++)
		{
			roots[i] = base(cos(ang * i), sin(ang * i));
		}
	}

	void fft(vector<base> &a, bool inv)
	{
		int n = a.size();
		for(int i=1; i<n; i++)
		{
			if(i < inv_fft[i]) swap(a[i], a[inv_fft[i]]);
		}
		

		for(int i=2; i<=n; i<<=1)
		{
			int step = n / i;
			for(int j=0; j<n; j+=i)
			{
				for(int k=0; k<i/2; k++)
				{
					base u = a[j+k], v = a[j+k+i/2] * roots[step * k];
					a[j+k] = u+v;
					a[j+k+i/2] = u-v;
				}
			}
		}
		if(inv) for(int i=0; i<n; i++) a[i] /= n;
	}

	void fft2D(vector<vector<base>> &a, bool inv) {
		// fft rows
		preCalcFFT(a[0].size(), inv);
		for (int _row = 0; _row < a.size(); _row++) {
			fft(a[_row], inv);
		}

		// fft columns
		preCalcFFT(a.size(), inv);
		for (int _col = 0; _col < a[0].size(); _col++) {
			vector<base> tmp;
			for (int i = 0; i < a.size(); i++) {
				tmp.push_back(a[i][_col]);
			}
			fft(tmp, inv);
			for (int i = 0; i < a.size(); i++) {
				a[i][_col] = tmp[i];
			}
		}
	}

	vector<vector<base>> multiply(vector<vector<base>> &v, vector<vector<base>> &w)
	{
		int _row = 1;
		while (_row < v.size() + w.size()) _row <<= 1;

		vector<vector<base>> fv(_row), fw(_row);
		
		int _colv = 1, _colw = 1;
		for (int i = 0; i <= _row - 1; i++) {
			if (i < v.size()) fv[i] = vector<base>(v[i].begin(), v[i].end()), _colv = max(_colv, (int) v[i].size());
			if (i < w.size()) fw[i] = vector<base>(w[i].begin(), w[i].end()), _colw = max(_colw, (int) w[i].size());
		}

		int _col = 1;
		while(_col < _colv + _colw) _col <<= 1;
		for (int i = 0; i <= _row - 1; i++) {
			fv[i].resize(_col);
			fw[i].resize(_col);
		}

		fft2D(fv, 0);
		fft2D(fw, 0);

		for (int i = 0; i <= _row - 1; i++) {
			for (int j = 0; j <= _col - 1; j++) { 
				fv[i][j] *= fw[i][j];
			}
		}
		
		fft2D(fv, 1);

		return fv;
	}
	vector<vector<long long>> multiply(vector<vector<long long>> &v, vector<vector<long long>> &w, long long mod)
	{
		int _row = 1, _colv = 1, _colw = 1, _col = 1;
		for (int i = 0; i < v.size(); i++) _colv = max(_colv, (int) v[i].size());
		for (int i = 0; i < w.size(); i++) _colw = max(_colw, (int) w[i].size());
		while (_row < v.size() + w.size()) _row <<= 1;
		while (_col < _colw + _colv) _col <<= 1;
		vector<vector<base>> v1(_row), v2(_row), r1(_row), r2(_row);
		for (int i = 0; i <= _row - 1; i++) {
			v1[i].resize(_col);
			r1[i].resize(_col);
			v2[i].resize(_col);
			r2[i].resize(_col);
		}

		for (int i = 0; i < v.size(); i++) {
			for (int j = 0; j < v[i].size(); j++) {
				v1[i][j] = base(v[i][j] >> 15, v[i][j] & 32767);
			}
		}

		for (int i = 0; i < w.size(); i++) {
			for (int j = 0; j < w[i].size(); j++) {
				v2[i][j] = base(w[i][j] >> 15, w[i][j] & 32767);
			}
		}

		fft2D(v1, 0);
		fft2D(v2, 0);

		// multiply 2 ffts
		for(int i=0; i<_row; i++)
		{
			for (int j = 0; j < _col; j++) {
				int i1 = (i ? (_row - i) : i);
				int j1 = (j ? (_col - j) : j);
				base ans1 = (v1[i][j] + conj(v1[i1][j1])) * base(0.5, 0);
				base ans2 = (v1[i][j] - conj(v1[i1][j1])) * base(0, -0.5);
				base ans3 = (v2[i][j] + conj(v2[i1][j1])) * base(0.5, 0);
				base ans4 = (v2[i][j] - conj(v2[i1][j1])) * base(0, -0.5);
				r1[i][j] = (ans1 * ans3) + (ans1 * ans4) * base(0, 1);
				r2[i][j] = (ans2 * ans3) + (ans2 * ans4) * base(0, 1);
			}
		}

		fft2D(r1, 1);
		fft2D(r2, 1);

		_col = _colv + _colw - 1;
		_row = v.size() + w.size() - 1;
		
		vector<vector<long long>> ret(_row);
		for(int i=0; i<_row; i++)
		{
			ret[i].resize(_col);
			for (int j = 0; j < _col; j++) {
				long long av = (long long)round(r1[i][j].real());
				long long bv = (long long)round(r1[i][j].imag()) + (long long)round(r2[i][j].real());
				long long cv = (long long)round(r2[i][j].imag());
				av %= mod, bv %= mod, cv %= mod;
				ret[i][j] = (av << 30) + (bv << 15) + cv;
				ret[i][j] = (ret[i][j] % mod + mod) % mod;
			}
		}
		return ret;
	}
}
