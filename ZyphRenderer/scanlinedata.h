#ifndef _SCANLINEDATA_H_
#define _SCANLINEDATA_H_

class ScanLineData 
{
public:
	int currentY;
	float ndotla;
	float ndotlb;
	float ndotlc;
	float ndotld;

	float ua;
	float ub;
	float uc;
	float ud;

	float va;
	float vb;
	float vc;
	float vd;

	ScanLineData();
	virtual ~ScanLineData();
};


#endif // !_SCANLINEDATA_H_