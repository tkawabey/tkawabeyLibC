#include "xlib/image/ximageconv.h"
#include "xlib/xeception.h"
#include <math.h>

const int ScaleFactor = 12 ;
const int ScaleValue =(1<<ScaleFactor);
const int Rounding =(1<<(ScaleFactor-1));


#define XIMAGE_DBL_MAX         1.7976931348623158e+308 /* max value */


int XImageConverter::Scaled(double value)
{
	return(int)(ScaleValue * value);
}
XIMGE_SAMPLE XImageConverter::RgbToY(XIMGE_SAMPLE red, XIMGE_SAMPLE green, XIMGE_SAMPLE blue)
{
	int result =(Scaled(0.299) * red
			+ Scaled(0.587) * green + Scaled(0.114) * blue
			+ Rounding) >> ScaleFactor;

	if(result > XIMGE_MAXSAMPLEVALUE) {
		result = XIMGE_MAXSAMPLEVALUE;
	} else
	if(result < XIMGE_MINSAMPLEVALUE) {
		result = XIMGE_MINSAMPLEVALUE;
	}
	return result ;
}
/*
	XImageConverter
*/

XImageConverter::XImageConverter()
{
	int minpal=0;

	m_ProgressFunction = NULL;
	m_pProgressData = NULL;

	m_nIndexVolume        = 256;
	m_nPictureMask        = PictureMask_8Bits;
	m_nDitherMode         = mdDitherMode_None;
    m_nPalettePrecision   = PalettePrecision_888;
	m_nCrMag = 10 * 5 + 50 + 5;
	m_nCgMag = 10 * 5 + 50 + 25;
	m_nCbMag = 10 * 5 + 50 - 15;
	m_nDitherLevel        = 7;
	m_bAdd16Pallete       = FALSE;
	m_nTransparencyColor  = -1;

	if( m_bAdd16Pallete == TRUE )
		minpal += 16;
	if( m_nTransparencyColor >= 0 )
		minpal++;
	if( m_nIndexVolume < minpal )
		m_nIndexVolume = minpal;

}
XImageConverter::~XImageConverter()
{

}
bool XImageConverter::convertGrayscale(XImage &image)
{
	XImage dst;
	unsigned int ii, jj;

	dst.SetSize(1 << 8, 8, image.GetWidth(), image.GetHeight());
	// グレースケール用カラーマップの作成
	for(ii =0;  ii < (1 << 8); ++ ii) {
		dst.GetColorMap(ii).red   =(XUBYTE1)ii;
		dst.GetColorMap(ii).green =(XUBYTE1)ii;
		dst.GetColorMap(ii).blue  =(XUBYTE1)ii;
	}
	for(ii = 0 ; ii < image.GetWidth(); ++ ii) {
		for(jj = 0 ; jj < image.GetHeight(); ++ jj) {
			XIMGE_SAMPLE red ;
			XIMGE_SAMPLE green ;
			XIMGE_SAMPLE blue ;
			image.GetRgb(jj, ii, red, green, blue);
			XIMGE_SAMPLE yvalue = RgbToY(red, green, blue);
			dst.SetPixelNo(ii, jj, yvalue);
		}
	}
	image = dst;
	return true;
}




XImageConverter::ClorSpace::ClorSpace()
{
}
XImageConverter::ClorSpace::~ClorSpace()
{
}
int XImageConverter::ClorSpace::MaskRGB(int rgb)
{
	int r,g,b,vr,vg,vb,nrgb,rrgb=0,nr,ng,nb;
    GCS gcs1,gcs2;
	double nd,d;

    ConvRGBtoGCS(&gcs1,rgb);
    b=(rgb & 0xff);
    g=(rgb & 0xff00)>>8;
    r=(rgb & 0xff0000)>>16;

	d = XIMAGE_DBL_MAX;
    for (vr=0;vr<2;vr++) for (vg=0;vg<2;vg++) for (vb=0;vb<2;vb++)
	{
    	nr=MaskRGBsub(m_MaskRTable,m_MaskRCount,r,vr);
    	ng=MaskRGBsub(m_MaskGTable,m_MaskGCount,g,vg);
    	nb=MaskRGBsub(m_MaskBTable,m_MaskBCount,b,vb);
        ConvRGBtoGCS(&gcs2,nrgb=((nb)|(ng<<8)|(nr<<16)));
        if (d>(nd=DistanceGCS(&gcs1,&gcs2)))
		{
        	rrgb=nrgb;
            d=nd;
        }
    }
	return rrgb;
}

double XImageConverter::ClorSpace::DistanceGCSd(GCS *g1, int x2, int y2, int z2) const
{
	double x,y,z;

	z=(g1->Z - z2);
	x=(g1->X - x2);
	y=(g1->Y - y2);

	return (x*x)+(y*y)+(z*z);
}
double XImageConverter::ClorSpace::DistanceGCS(GCS *g1, GCS *g2) const
{
	double x,y,z;

	z=(g1->Z - g2->Z);
	x=(g1->X - g2->X);
	y=(g1->Y - g2->Y);

	return (x*x)+(y*y)+(z*z);
}

int XImageConverter::ClorSpace::ConvGCStoRGB(int X, int Y, int Z) const
{
	#define colortrim2(n) (max(0.0,min(1.0,n)))

	double R,G,B,x,y,z,rr,gg,bb,r3,g3,b3;
	int r,g,b;

	x=m_M2.m11*(double)X+m_M2.m12*(double)Y;
	y=m_M2.m21*(double)X+m_M2.m22*(double)Y;
	z=Z;
	// ITU.BT-601 Y'CbCr 80%
	R= z*0.9998247134019967+x*-0.000044452970991771824+y* 1.7528659800326482;
	G= z*1.0000893144198046+x*-0.4320813565838843+     y*-0.89314419804726;
	B= z*1.0000000115762946+x* 2.213731098385467+      y*-0.00011576294529099052;

	r3=colortrim2(R/255/16384);
	if (r3>=0.0540269630587776405948631399435028)
		rr=pow(r3,2.2/3.)*255;
	else
		rr=pow(r3/0.142913647595774056881018286010431,2.2)*255;

	g3=colortrim2(G/255/16384);
	if (g3>=0.0540269630587776405948631399435028)
		gg=pow(g3,2.2/3.)*255;
	else
		gg=pow(g3/0.142913647595774056881018286010431,2.2)*255;

	b3=colortrim2(B/255/16384);
	if (b3>=0.0540269630587776405948631399435028)
		bb=pow(b3,2.2/3.)*255;
	else
		bb=pow(b3/0.142913647595774056881018286010431,2.2)*255;

	r=(int)floor(rr+0.5);
	g=(int)floor(gg+0.5);
	b=(int)floor(bb+0.5);

	if (r<0)
		r=0;
	else if (r>255)
		r=255;
	if (g<0)
		g=0;
	else if (g>255)
		g=255;
	if (b<0)
		b=0;
	else if (b>255)
		b=255;

	return (b & 0xff) | ((g & 0xff)<<8) | ((r & 0xff) <<16);
}

void XImageConverter::ClorSpace::ConvRGBtoGCS(GCS *gcs, unsigned int rgb) const
{
	double r,g,b,n1,n2,n3,n22,n33;

	b=m_dGtable[(rgb & 0xff)];
	g=m_dGtable[(rgb & 0xff00)>>8];
	r=m_dGtable[(rgb & 0xff0000)>>16];

	// ITU.BT-601 Y'CbCr 80%
	n1= (double)r*0.2989+(double)g*0.5866+(double)b*0.1145;//Y
	n2=-(double)r*0.1350-(double)g*0.2650+(double)b*0.4000;//Cb
	n3= (double)r*0.4000-(double)g*0.3346-(double)b*0.0653;//Cr

	n22=m_M1.m11 * n2 + m_M1.m12 * n3;
	n33=m_M1.m21 * n2 + m_M1.m22 * n3;

	gcs->Z=(int)floor(n1 *16384.0*255.0+.5);
	gcs->X=(int)floor(n22*16384.0*255.0+.5);
	gcs->Y=(int)floor(n33*16384.0*255.0+.5);

}

void XImageConverter::ClorSpace::Create(int b, int r, int g, int p)
{

	int i;
	int fr=256,fg=256,fb=256;
	double n,m;

	switch (p)
	{
		case 0://web
			fr=fg=fb=6;
			break;
		case 1://333
			fr=fg=8, fb=4;
			break;
		case 2://333
			fr=fg=fb=8;
			break;
		case 3://444
			fr=fg=fb=16;
			break;
		case 4://555
			fr=fg=fb=32;
			break;
		case 5://655
			fr=fb=32; fg=64;
			break;
		case 6://666
			fr=fg=fb=64;
			break;
		case 7://777
			fr=fg=fb=128;
			break;
		case 8://888
			fr=fg=fb=256;
			break;
	}

	{
		m_MaskRCount=fr;
		m_MaskGCount=fg;
		m_MaskBCount=fb;
		for (i=0;i<fr;i++)
			m_MaskRTable[i] = min((int)floor((double)i*256.0/(double)(fr-1)),255);
		for (i=0;i<fg;i++)
			m_MaskGTable[i] = min((int)floor((double)i*256.0/(double)(fg-1)),255);
		for (i=0;i<fb;i++)
			m_MaskBTable[i] = min((int)floor((double)i*256.0/(double)(fb-1)),255);
	}
/*
	for (i=0;i<256;i++) {
		gammatable[i]=pow((double)i / 255.0,GM);
	}
*/
	for (i=0;i<256;i++)
	{
		n=i/255.;

		if (n>=0.117647058823529411764705882352941)
			m=pow(n,3./2.2);
		else
			m=pow(n,1./2.2)*0.142913647595774056881018286010431;

		m_dGtable[i]=m;

		if (n>=0.20198703620241161355887485889543)
			m=pow(n,3./2.2);
		else
			m=pow(n,2.2)*0.112905177848716256193056297677849/0.029628638340582134493141518902643;
		m_dR2table[i]=m;

		if (n>=0.148670653233635731053932622069695)
			m=pow(n,3./2.2);
		else
			m=pow(n,2.2)*0.0743389203713377581101520770121405/0.0150971701329696556426866689396522;
		m_dG2table[i]=m;

		if (n>=0.312422190652192596516505331287941)
			m=pow(n,3./2.2);
		else
			m=pow(n,2.2)*0.204649951175241102069736654437528/0.07734497816593886462882096069869;
		m_dB2table[i]=m;

	}


	MakeMatrix(&m_M1,&m_M2,(double)b/100.0,(double)r/100.0,(double)g/100.0);

}

bool XImageConverter::ClorSpace::MakeMatrix(MRX *a, MRX *b, double Cbm, double Crm, double Cgm)
{
	double Cbx= 0.5000, Cby= 0.0816;
	double Crx=-0.1687, Cry=-0.5000;
	double Cgx= 0.3312, Cgy=-0.4183;
	double d;

	MRX M,N;

	double Csin,Ccos,Clen;

	// 青変換
	Clen=sqrt(Cbx*Cbx + Cby*Cby);
	if (Clen==0)
		goto diverr;
	Csin=Cby/Clen;
	Ccos=Cbx/Clen;

	M.m11=(Cbm-1.0)*Ccos*Ccos+1,	M.m12=(1.0-Cbm)*Ccos*Csin;
	M.m21=(1.0-Cbm)*Ccos*Csin,		M.m22=(Cbm-1.0)*Csin*Csin+1;

	// 赤変換
	Clen=sqrt(Crx*Crx + Cry*Cry);
	if (Clen==0)
		goto diverr;
	Csin=Cry/Clen;
	Ccos=Crx/Clen;

	N.m11=(Crm-1.0)*Ccos*Ccos+1,	N.m12=(1.0-Crm)*Ccos*Csin;
	N.m21=(1.0-Crm)*Ccos*Csin,		N.m22=(Crm-1.0)*Csin*Csin+1;

	MakeMatrixsub(&M,&N);

	// 緑変換
	Clen=sqrt(Cgx*Cgx + Cgy*Cgy);
	if (Clen==0)
		goto diverr;
	Csin=Cgy/Clen;
	Ccos=Cgx/Clen;

	N.m11=(Cgm-1.0)*Ccos*Ccos+1,	N.m12=(1.0-Cgm)*Ccos*Csin;
	N.m21=(1.0-Cgm)*Ccos*Csin,		N.m22=(Cgm-1.0)*Csin*Csin+1;

	MakeMatrixsub(&M,&N);

	a->m11=M.m11, a->m12=M.m12;
	a->m21=M.m21, a->m22=M.m22;

	d=M.m11*M.m22-M.m12*M.m21;
	if (d==0)
		goto diverr;

	b->m11= M.m22/d, b->m12=-M.m12/d;
	b->m21=-M.m21/d, b->m22= M.m11/d;

	return true;

diverr:
	a->m11=1.0, a->m12=0.0;
	a->m21=0.0, a->m22=1.0;

	b->m11=1.0, b->m12=0.0;
	b->m21=0.0, b->m22=1.0;

	return false;
}

int XImageConverter::ClorSpace::MaskRGBsub(int m[], int mc, int in, int s)
{
	int r,i;
	if (s==0)
	{
    	r=0;
        for (i=0;i<mc;i++)
			if (r<m[i] && m[i]<=in)
				r=m[i];
    }
    else
	{
    	r=255;
        for (i=0;i<mc;i++)
			if (r>m[i] && m[i]>=in)
				r=m[i];
    }
    return r;
}

void XImageConverter::ClorSpace::MakeMatrixsub(MRX *M, MRX *N)
{
	MRX a;

	a.m11 = N->m11 * M->m11 + N->m12 * M->m21;
	a.m12 = N->m11 * M->m12 + N->m12 * M->m22;
	a.m21 = N->m21 * M->m11 + N->m22 * M->m21;
	a.m22 = N->m21 * M->m12 + N->m22 * M->m22;

	M->m11 = a.m11;
	M->m12 = a.m12;
	M->m21 = a.m21;
	M->m22 = a.m22;
}


//
//
//
void  XImageConverter::DivideBox::DivHsort(MED **ab, int n)
{
	MED** a=ab-1;
	int i, j, k;
	MED* x;

	for (k = n / 2; k >= 1; k--)
	{
		i = k;  x = a[i];
		while ((j = 2 * i) <= n)
		{
			if (j < n && ((*(a+j))->flag < (*(a+j+1))->flag))
				j++;
			if (x->flag >= (*(a+j))->flag)
				break;
			a[i] = a[j];
			i = j;
		}
		a[i] = x;
	}
	while (n > 1)
	{
		x = a[n];  a[n] = a[1];  n--;
		i = 1;
		while ((j = 2 * i) <= n)
		{
			if (j < n && ((*(a+j))->flag < (*(a+j+1))->flag))
				j++;
			if (x->flag >= (*(a+j))->flag)
				break;
			a[i] = a[j];
			i = j;
		}
		a[i] = x;
	}

}
XImageConverter::DivideBox::DivideBox()
{
}
XImageConverter::DivideBox::~DivideBox()
{
}
#define dsqr(a) ((a)*(a))
//---------------------------------------------------------------------------
/*
static int qsortflag(const void *a,const void *b)
{

	double m=(( (*((MED **)a)) ->flag)-((*((MED **)b))->flag));
	if (m>0) return 1;
	if (m<0) return -1;
	return 0;

}
*/
//---------------------------------------------------------------------------
// return : 若順ブロックの要素個数
int XImageConverter::DivideBox::FindSquareDiv(MED **ix, int st, int n)
{
#define FSDtype 0
//////////////////////////////////////////////////
#if FSDtype==0
	struct GCSd
	{
		double X,Y,Z;
	};
	struct STC
	{
		GCSd total;
		double count;
		GCSd sqr;
	};

	MED **idx;
	STC L,R;
	int i,mk=0;
	double mn=XIMAGE_DBL_MAX,m;
	GCSd a,b,d,e;
	double x,y,z,c;
	double xc,yc,zc;
	double mc;


	//初期データ作成------------------------------------------

	L.count=0;
	L.total.X=0, L.sqr.X=0;
	L.total.Y=0, L.sqr.Y=0;
	L.total.Z=0, L.sqr.Z=0;
	R.count=0;
	R.total.X=0, R.sqr.X=0;
	R.total.Y=0, R.sqr.Y=0;
	R.total.Z=0, R.sqr.Z=0;
	idx=ix+st;
	for (i=0;i<n;i++)
	{
		x=(**idx).gcs.X;
		y=(**idx).gcs.Y;
		z=(**idx).gcs.Z;
		c=(**idx).cnt;
		xc=x*c;
		yc=y*c;
		zc=z*c;

		R.count+=c;
		R.total.X+=xc;
		R.total.Y+=yc;
		R.total.Z+=zc;

		R.sqr.X+=xc*x;
		R.sqr.Y+=yc*y;
		R.sqr.Z+=zc*z;

		idx++;
	}
	a.X=R.total.X/R.count;
	a.Y=R.total.Y/R.count;
	a.Z=R.total.Z/R.count;

	R.sqr.X+=(double)a.X*a.X*R.count - 2.*a.X*R.total.X;
	R.sqr.Y+=(double)a.Y*a.Y*R.count - 2.*a.Y*R.total.Y;
	R.sqr.Z+=(double)a.Z*a.Z*R.count - 2.*a.Z*R.total.Z;

	//ループ---------------------------------------------------
	idx=ix+st;
	for(i=1;i<n;i++)
	{
		x=(**idx).gcs.X;
		y=(**idx).gcs.Y;
		z=(**idx).gcs.Z;
		c=(**idx).cnt;
		xc=x*c;
		yc=y*c;
		zc=z*c;

		//Left side
		mc=max(L.count,1.);
		a.X=L.total.X/mc;
		a.Y=L.total.Y/mc;
		a.Z=L.total.Z/mc;
		mc=max(L.count+c,1.);
		d.X=(b.X=((e.X=(L.total.X+xc))/mc))-a.X;
		d.Y=(b.Y=((e.Y=(L.total.Y+yc))/mc))-a.Y;
		d.Z=(b.Z=((e.Z=(L.total.Z+zc))/mc))-a.Z;
		L.sqr.X+=d.X*d.X*L.count;
		L.sqr.Y+=d.Y*d.Y*L.count;
		L.sqr.Z+=d.Z*d.Z*L.count;
		L.total.X=e.X;
		L.total.Y=e.Y;
		L.total.Z=e.Z;
		L.count=mc;
		L.sqr.X+=dsqr(x-b.X)*c;
		L.sqr.Y+=dsqr(y-b.Y)*c;
		L.sqr.Z+=dsqr(z-b.Z)*c;

		//Right side
		mc=max(R.count,1.);
		a.X=R.total.X/mc;
		a.Y=R.total.Y/mc;
		a.Z=R.total.Z/mc;
		mc=max(R.count-c,1.);
		d.X=(b.X=((e.X=(R.total.X-x*c))/mc))-a.X;
		d.Y=(b.Y=((e.Y=(R.total.Y-y*c))/mc))-a.Y;
		d.Z=(b.Z=((e.Z=(R.total.Z-z*c))/mc))-a.Z;
		R.sqr.X+=d.X*d.X*R.count;
		R.sqr.Y+=d.Y*d.Y*R.count;
		R.sqr.Z+=d.Z*d.Z*R.count;
		R.total.X=e.X;
		R.total.Y=e.Y;
		R.total.Z=e.Z;
		R.count=mc;
		R.sqr.X-=dsqr(x-b.X)*c;
		R.sqr.Y-=dsqr(y-b.Y)*c;
		R.sqr.Z-=dsqr(z-b.Z)*c;

//		m=dsqr(L.sqr.X+L.sqr.Y+L.sqr.Z)+dsqr(R.sqr.X+R.sqr.Y+R.sqr.Z);
		m=(L.sqr.X+L.sqr.Y+L.sqr.Z)+(R.sqr.X+R.sqr.Y+R.sqr.Z);
		if (mn>m) {
			mn=m,	mk=i;
		}
		idx++;

	}

	return mk;
#endif
#if FSDtype==1
//////////////////////////////////////////////////
	struct GCSd
	{
		double F;//X,Y,Z;
	};
	struct STC
	{
		GCSd total;
		double count;
		GCSd sqr;
	};

	MED *idx;
	STC L,R;
	int i,mk=0;
	double mn=XIMAGE_DBL_MAX,m;
	GCSd a,b,d,e;
	double f,c;
	double fc;
	double mc;


	//初期データ作成------------------------------------------

	L.count=0;
	L.total.F=0;
	L.sqr.F=0;
	R.count=0;
	R.total.F=0;
	R.sqr.F=0;
	idx=ix+st;
	for (i=0;i<n;i++)
	{
		f=(*idx).flag;
		c=(*idx).cnt;
		fc=f*c;

		R.count+=c;
		R.total.F+=fc;

		R.sqr.F+=fc*f;

		idx++;
	}
	a.F=R.total.F/R.count;

	R.sqr.F+=(double)a.F*a.F*R.count - 2.*a.F*R.total.F;

	//ループ---------------------------------------------------
	idx=ix+st;
	for (i=1;i<n;i++)
	{
		f=(*idx).flag;
		c=(*idx).cnt;
		fc=f*c;

		//Left side
		mc=max(L.count,1.);
		a.F=L.total.F/mc;
		mc=max(L.count+c,1.);
		d.F=(b.F=((e.F=(L.total.F+fc))/mc))-a.F;
		L.sqr.F+=d.F*d.F*L.count;
		L.total.F=e.F;
		L.count=mc;
		L.sqr.F+=dsqr(f-b.F)*c;

		//Right side
		mc=max(R.count,1.);
		a.F=R.total.F/mc;
		mc=max(R.count-c,1.);
		d.F=(b.F=((e.F=(R.total.F-f*c))/mc))-a.F;
		R.sqr.F+=d.F*d.F*R.count;
		R.total.F=e.F;
		R.count=mc;
		R.sqr.F-=dsqr(f-b.F)*c;

//		m=dsqr(L.sqr.X+L.sqr.Y+L.sqr.Z)+dsqr(R.sqr.X+R.sqr.Y+R.sqr.Z);
		m=(L.sqr.F)+(R.sqr.F);
		if (mn>m)
		{
			mn=m,	mk=i;
		}
		idx++;

	}

	return mk;
#endif
}

void XImageConverter::DivideBox::Jacobi(int n,
								  double a[][3],
								  double u[][3],
								  double eps)
{
	bool finish;
	int m,i,j,k;
	double p,q,t,s2,c2,c,s,r;

	finish=false;

	for (m=0;m<10 && finish==false;m++)
	{
		finish = true;

		for (i=0;i<n-1;i++)
		{
			for (j=i+1;j<n;j++)
			{
				if (fabs(a[i][j])<eps) continue;
				finish=false;

				p = (a[i][i] - a[j][j]) / 2;
				q = a[i][j];
				t = p / q;
				s2 = 1 / sqrt(1 + t * t);
				if (q < 0) s2 = -s2;
				c2 = t * s2;
				if (c2 > 0)
				{
					c = sqrt((1 + c2) / 2);
					s = s2 / c / 2;
				}
				else
				{
					s = sqrt((1 - c2) / 2);
					c = s2 / s / 2;
				}
				r = a[i][i] + a[j][j];
				a[i][i] = r / 2 + p * c2 + q * s2;
				a[j][j] = r - a[i][i];
				a[i][j] = a[j][i] = 0;

				for (k = 0; k < n; k++)
				{
					if ((k != i) && (k != j))
					{
						p = a[k][i];
						q = a[k][j];
						a[k][i] = a[i][k] =  p * c + q * s;
						a[k][j] = a[j][k] = -p * s + q * c;
					}
				}
				for (k = 0; k < n; k++)
				{
					p = u[k][i];
					q = u[k][j];
					u[k][i] =  p * c + q * s;
					u[k][j] = -p * s + q * c;
				}
			}
		}
	}
}

double XImageConverter::DivideBox::Setrma(MED **medbuf, 
									int bs, 
									int bn, 
									RMA *prin, 
									RMA *std, 
									RMA *avg)
{
	double c;
	double d;
	double cnt=0;
	int i,j;

	avg->x=0;
	avg->y=0;
	avg->z=0;
	avg->c=0;
	if (bn<=0) return 0;

	for (i=0;i<bn;i++)
	{
		cnt+=c=(*medbuf[bs+i]).cnt;
		avg->x+=(double)(*medbuf[bs+i]).gcs.X * c;
		avg->y+=(double)(*medbuf[bs+i]).gcs.Y * c;
		avg->z+=(double)(*medbuf[bs+i]).gcs.Z * c;
	}
	avg->x/=cnt;
	avg->y/=cnt;
	avg->z/=cnt;
	avg->c=std->c=prin->c=cnt;
	//共分散行列作成
	{
		double d[3];
		int dx,dy;
		for (i=0;i<bn;i++) {
			c=(*medbuf[bs+i]).cnt;
			d[0]=(*medbuf[bs+i]).gcs.X - avg->x;
			d[1]=(*medbuf[bs+i]).gcs.Y - avg->y;
			d[2]=(*medbuf[bs+i]).gcs.Z - avg->z;

			for (dx=0;dx<3;dx++)
				for (dy=0;dy<3;dy++)
				{
					m_Jca[dx][dy]+=d[dx]*d[dy]*c;
				};
		}
		for (dx=0;dx<3;dx++)
			for (dy=0;dy<3;dy++)
				m_Jca[dx][dy]/=avg->c;

		for (dx=0;dx<3;dx++)
		{
			for (dy=0;dy<3;dy++)
				m_Jcw[dx][dy]=0;
			m_Jca[dx][dx]=1;
		}
	}

	std->x=sqrt(m_Jca[0][0]);
	std->y=sqrt(m_Jca[1][1]);
	std->z=sqrt(m_Jca[2][2]);

	Jacobi(3, m_Jca, m_Jcw, pow(10,-16));
//	Jacobi(3, jca, jcw, pow(10,-8));

	j=2; d=m_Jca[j][j];
	for (i=0;i<3;i++)
	{
		if (d<m_Jca[i][i])
		{
			j=i;
			d=m_Jca[i][i];
		}
	}

	prin->x=m_Jcw[0][j];
	prin->y=m_Jcw[1][j];
	prin->z=m_Jcw[2][j];

	return d;
}
void XImageConverter::DivideBox::MakeMEDbox(MEDBOX *medbox, 
									  MED **medbuf, 
									  int ad, 
									  int bs, 
									  int bn,
									  ClorSpace *cs)
{
	int i;
	int mx,my,mz;
	double st;
	RMA prin,std,avg;
//	double dmn,dmx,dmw
//	double m;

	medbox[ad].bufstart=bs;
	medbox[ad].bufnum=bn;

	medbox[ad].firstd=Setrma(medbuf,bs,bn,&prin,&std,&avg);
	memcpy(&medbox[ad].prin,&prin,sizeof(RMA));
	memcpy(&medbox[ad].std,&std,sizeof(RMA));
	memcpy(&medbox[ad].avg,&avg,sizeof(RMA));

	mx=(int)floor(avg.x+0.5);
	my=(int)floor(avg.y+0.5);
	mz=(int)floor(avg.z+0.5);

	medbox[ad].rgb=cs->MaskRGB(cs->ConvGCStoRGB(mx,my,mz));

	medbox[ad].mid.X=mx;
	medbox[ad].mid.Y=my;
	medbox[ad].mid.Z=mz;


	// 歪み
	st=0;
	for (i=bs;i<bs+bn;i++)
		st+=(cs->DistanceGCSd(&((*medbuf[i]).gcs),mx,my,mz))*(double)(*medbuf[i]).cnt;
//		st+=sqrtz(distanceGCSd(&(medbuf[i].gcs),mx,my,mz))*medbuf[i].cnt;
	medbox[ad].strain=st;

}
bool XImageConverter::DivideBox::divideBox(MEDBOX *medbox,
									 MED **medbuf,
									 int mxa,
									 int oi,
									 ClorSpace *cs)
{
	int bs,bn,j;
//	double m;
	RMA prin,std,avg;


	bs=medbox[mxa].bufstart;
	bn=medbox[mxa].bufnum;
	if (bn==1)
		goto oneerr;

	// 主成分軸抽出
//	get_rma(bs,bn,&prin,&avg);
	memcpy(&prin,&medbox[mxa].prin,sizeof(RMA));
	memcpy(&std,&medbox[mxa].std,sizeof(RMA));
	memcpy(&avg,&medbox[mxa].avg,sizeof(RMA));

#define nBOXDEV 1
	// ボックス分割マーク
//-------------------------------
#if nBOXDEV==0
	{
		double m=avg.x*prin.x + avg.y*prin.y + avg.z*prin.z;
		int mm=bs+bn;
		for (j=bs;j<mm;j++)
			(*medbuf[j]).flag=	(*medbuf[j]).gcs.X*prin.x+
								(*medbuf[j]).gcs.Y*prin.y+
								(*medbuf[j]).gcs.Z*prin.z;
		qsort(&medbuf[bs],bn,sizeof(MED*),qsortflag);

		j=bs,mm=bs+bn;  while (j<mm && (*medbuf[j]).flag<m) j++;
		j-=bs;

	}
#endif
//-------------------------------
#if nBOXDEV==1
	{
		int mm=bs+bn;
		for (j=bs;j<mm;j++)
			(*medbuf[j]).flag=	(*medbuf[j]).gcs.X*prin.x+
								(*medbuf[j]).gcs.Y*prin.y+
								(*medbuf[j]).gcs.Z*prin.z;
//		qsort(&medbuf[bs],bn,sizeof(MED*),qsortflag);
		DivHsort(&medbuf[bs],bn);
		j=FindSquareDiv(medbuf,bs,bn);

	//	MessageBox(Application->Handle,
	//		(AnsiString(j)+"/"+AnsiString(bn)).c_str(),
	//		"Total Box",MB_ICONINFORMATION);


	}
#endif
//-------------------------------
		if (j<1 || bn-j<1) {
			if (bn==2) j=1;
			else goto oneerr;
		}



	MakeMEDbox(medbox,medbuf,mxa,bs,j,cs);
	MakeMEDbox(medbox,medbuf,oi,bs+j,bn-j,cs);
	return true;
oneerr:
	MakeMEDbox(medbox,medbuf,mxa,bs,bn,cs);
	medbox[mxa].strain=0;
	return false;
}



//
//
//
int XImageConverter::m_fbpattern[2][3]={
	{ 3,5,1 },
	{ 7,0,0 }
};
int XImageConverter::m_maempattern[3][5]={
	{ 1,3,5,3,1 },
    { 3,5,7,5,3 },
    { 5,7,0,0,0 }
};
#ifdef _DEBUG
#include <limits.h>
#define CRCPOLY1 0x04C11DB7UL
	/* x^{32}+x^{26}+x^{23}+x^{22}+x^{16}+x^{12}+x^{11]+
	   x^{10}+x^8+x^7+x^5+x^4+x^2+x^1+1 */
#define CRCPOLY2 0xEDB88320UL  /* 左右逆転 */
unsigned long crctable[UCHAR_MAX + 1];
static void makecrctable(void)
{
	unsigned int i, j;
	unsigned long r;

	for (i = 0; i <= UCHAR_MAX; i++)
	{
		r = i;
		for (j = 0; j < CHAR_BIT; j++)
			if(r & 1)
				r = (r >> 1) ^ CRCPOLY2;
			else       r >>= 1;
		crctable[i] = r;
	}
}
static unsigned long crc(int n, XUBYTE1 c[])
{
	unsigned long r;

	r = 0xFFFFFFFFUL;
	while (--n >= 0)
		r = (r >> CHAR_BIT) ^ crctable[(XUBYTE1)r ^ *c++];
	return r ^ 0xFFFFFFFFUL;
}
#endif
//////////////////////////////////////////////////////////////////////
static void  array_hsort(int *ab, int n)
{
	int* a=ab-1;
	int i, j, k;
	int x;

	for (k = n / 2; k >= 1; k--)
	{
		i = k;  x = a[i];
		while ((j = 2 * i) <= n)
		{
			if (j < n && (((*(a+j)) & 0xfeffffff) < ((*(a+j+1)) & 0xfeffffff)))
				j++;
			if ((x & 0xfeffffff) >= ((*(a+j)) & 0xfeffffff))
				break;
			a[i] = a[j];  i = j;
		}
		a[i] = x;
	}
	while (n > 1)
	{
		x = a[n];  a[n] = a[1];  n--;
		i = 1;
		while ((j = 2 * i) <= n)
		{
			if (j < n && (((*(a+j)) & 0xfeffffff) < ((*(a+j+1)) & 0xfeffffff)))
				j++;
			if ((x & 0xfeffffff) >= ((*(a+j)) & 0xfeffffff))
				break;
			a[i] = a[j];  i = j;
		}
		a[i] = x;
	}
}
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
/*
static int qsortsub(const void *a,const void *b)
{
	return ((*(int *)a) & 0xfeffffff)-((*(int *)b) & 0xfeffffff);
}
*/
//---------------------------------------------------------------------------
static int qsortpal(const void *a,const void *b)
{
	return (((XImageConverter::PALSTR *)a)->z)-(((XImageConverter::PALSTR *)b)->z);
}
//---------------------------------------------------------------------------
static int qsortanime(const void *a,const void *b)
{
	int n=(((XImageConverter::Anime2line *)a)->d)-(((XImageConverter::Anime2line *)b)->d);
	if (n>0) return 1;
	if (n<0) return -1;
	return (((XImageConverter::Anime2line *)a)->pal)-(((XImageConverter::Anime2line *)b)->pal);
}




void XImageConverter::SetProgressFunction(XIMAGE_COMV_PROGRESSFUNCTION func, void *data)
{
	m_ProgressFunction = func ;
	m_pProgressData = data ;
}
// 出力制度の取得＆設定
XImageConverter::PalettePrecision XImageConverter::GetPalettePrecision()
{
	return m_nPalettePrecision;
}
void XImageConverter::SetPalettePrecision(PalettePrecision nPalettePrecision)
{
	switch(nPalettePrecision)
	{
	case PalettePrecision_Web:
	case PalettePrecision_332:
	case PalettePrecision_333:
	case PalettePrecision_444:
	case PalettePrecision_555:
	case PalettePrecision_565:
	case PalettePrecision_666:
	case PalettePrecision_777:
	case PalettePrecision_888:
		break;
	default:
		XImageException::Throw(XIMG_ERR_INVALID_PALLET_PRECISION);
	}
	m_nPalettePrecision = nPalettePrecision;
}
// 入力制度の取得＆設定
XImageConverter::PictureMask XImageConverter::GetPicturMask()
{
	return m_nPictureMask;
}
void XImageConverter::SetPicturMask(PictureMask nPictureMask)
{
	switch(nPictureMask)
	{
	case PictureMask_5Bits:
	case PictureMask_6Bits:
	case PictureMask_7Bits:
	case PictureMask_8Bits:
		break;
	default:
		XImageException::Throw( XIMG_ERR_INVALID_PICTURE_MASK );
	}
	m_nPictureMask = nPictureMask;
}
// 減色エンジン赤(0-20)の取得＆設定
unsigned int XImageConverter::GetCrMag()
{
	return m_nCrMag;
}
void XImageConverter::SetCrMag(unsigned int nCrMag)
{
	if( nCrMag > 20 )
		XImageException::Throw( XIMG_ERR_INVALID_CRMA );
	m_nCrMag = nCrMag * 5 + 50 + 5;
}
// 減色エンジン緑(0-20)
unsigned int XImageConverter::GetCgMag()
{
	return m_nCgMag;
}
void XImageConverter::SetCgMag(unsigned int nCgMag)
{
	if( nCgMag > 20 )
		XImageException::Throw( XIMG_ERR_INVALID_CRMA );
	m_nCgMag = nCgMag * 5 + 50 + 25;
}
// 減色エンジン青(0-20)の取得＆設定
unsigned int XImageConverter::GetCbMag()
{
	return m_nCbMag;
}
void XImageConverter::SetCbMag(unsigned int nCbMag)
{
	if( nCbMag > 20 )
		XImageException::Throw( XIMG_ERR_INVALID_CRMA );
	m_nCbMag = nCbMag * 5 + 50 - 15;
}
// カラー数の取得＆設定
unsigned int XImageConverter::GetIndexVolume()
{
	return m_nIndexVolume;
}
void XImageConverter::SetIndexVolume(unsigned int nIndexVolume)
{
	int minpal=0;
	m_nIndexVolume = nIndexVolume;
	if( m_bAdd16Pallete == TRUE )
		minpal += 16;
	if( m_nTransparencyColor >= 0 )
		minpal++;
	if( m_nIndexVolume < minpal )
		m_nIndexVolume = minpal;
}
// ディザレベルの取得＆設定
unsigned int XImageConverter::GetDitherLevel()
{
	return m_nDitherLevel;
}
void XImageConverter::SetDitherLevel(unsigned int nDitherLevel)
{
	if( nDitherLevel > 10 )
		XImageException::Throw( XIMG_ERR_INVALID_DITHER_LEVEL );

	m_nDitherLevel = nDitherLevel;
}
// ディザモードの取得＆設定
XImageConverter::DitherMode XImageConverter::GetDitherMode()
{
	return m_nDitherMode;
}
void XImageConverter::SetDitherMode(DitherMode nDitherMode)
{
	switch(nDitherMode)
	{
	case mdDitherMode_None:
	case mdDitherMode_gPeanos:
	case mdDitherMode_ObservationalDiffusion:
	case mdDitherMode_AverageObservational:
	case mdDitherMode_Anime:
		break;
	default:
		XImageException::Throw( XIMG_ERR_INVALID_DITHER_MODE );
	}
	m_nDitherMode = nDitherMode;
}
// 固定16色付加の取得＆設定
BOOL XImageConverter::GetAdd16Pallete()
{
	return m_bAdd16Pallete;
}
void XImageConverter::SetAdd16Pallete(BOOL bAdd16Pallete)
{
	int minpal=0;

	m_bAdd16Pallete = bAdd16Pallete;

	if( m_bAdd16Pallete == TRUE )
		minpal += 16;
	if( m_nTransparencyColor >= 0 )
		minpal++;
	if( m_nIndexVolume < minpal )
		m_nIndexVolume = minpal;

}
// 透過パレットの取得＆設定
int XImageConverter::GetTransparencyColor()
{
	return m_nTransparencyColor;
}
void XImageConverter::SetTransparencyColor(int nTransparencyColor)
{
	int minpal=0;

	switch(nTransparencyColor)
	{
	case -1:
	case 0:
	case 1:
	case 2:
	case 3:
		break;
	default:
		XImageException::Throw( XIMG_ERR_INVALID_TRANCE_COLOR );
	}

	m_nTransparencyColor = nTransparencyColor;

	if( m_bAdd16Pallete == TRUE )
		minpal += 16;
	if( m_nTransparencyColor >= 0 )
		minpal++;
	if( m_nIndexVolume < minpal )
		m_nIndexVolume = minpal;
}




//---------------------------------------------------------------------------
// 最近距離パレット検索；キャッシング
//---------------------------------------------------------------------------
int XImageConverter::GetCubeadr(int x, int y, int z)
{
	int cx,cy,cz;

    cx=(x+CUBEH)/CUBESTEP;
    cy=(y+CUBEH)/CUBESTEP;
    cz=(z+CUBEZO)/CUBESTEP;

    if (cx<0 || cx>=CUBEX || cy<0 || cy>=CUBEY || cz<0 || cz>=CUBEZ) return -1;
    return (cz*CUBEY+cy)*CUBEX+cx;
}

int XImageConverter::FindClosepaletteSub(short *p, int x, int y, int z)
{
	double mn,mt;
    int k=0 ,kk;
    GCS gcs2;
    short int *pt=p;

	gcs2.X=x;
	gcs2.Y=y;
	gcs2.Z=z;

	mn=XIMAGE_DBL_MAX;
	while ((kk=*pt)>=0) {
		mt=m_ColorSpace.DistanceGCS(&gcs2,&m_GcsTable[kk]);
		if (mt<mn) {
			mn=mt;
			k=kk;
		}
		pt++;
	}
	return k;
}

void XImageConverter::MakeCube(int x, int y, int z)
{
	int c,a;
	short int *pt,i;
	double dt[256],d,mn;
	GCS gcs1;
	short int ap[256];
	int cx,cy,cz;

	a=GetCubeadr(x,y,z);

	cx=(x+CUBEH)/CUBESTEP;
	cy=(y+CUBEH)/CUBESTEP;
	cz=(z+CUBEZO)/CUBESTEP;
	d=sqrt((double)CUBESTEP*CUBESTEP*3);
	gcs1.X=(int)(CUBESTEP*cx+CUBESTEPH-CUBEH);
	gcs1.Y=(int)(CUBESTEP*cy+CUBESTEPH-CUBEH);
	gcs1.Z=(int)(CUBESTEP*cz+CUBESTEPH-CUBEZO);

	mn=dt[0]=sqrt((double)m_ColorSpace.DistanceGCS(&gcs1,&m_GcsTable[0]));
	for (i=1;i<m_nMakePalettEnum;i++) {
		if (mn>(dt[i]=sqrt((double)m_ColorSpace.DistanceGCS(&gcs1,&m_GcsTable[i])))) {
			mn=dt[i];
		}
	}
	c=0;
	for (i=0;i<m_nMakePalettEnum;i++) {
		if (dt[i]<mn+d) {
			ap[c++]=i;
		}
	}
	pt=m_pQcube[a]=(short int*)malloc((c+1)*sizeof(short int));
	for (i=0;i<c;i++) pt[i]=ap[i];
	pt[c]=-1;

}

int XImageConverter::FindClosepalette(int x, int y, int z)
{
	int k,j,i;
	GCS gcs2;
	double mn,mt;

	i=GetCubeadr(x,y,z);
	if (i<0) {
		gcs2.X=x;
		gcs2.Y=y;
		gcs2.Z=z;
		mn=m_ColorSpace.DistanceGCS(&gcs2,&m_GcsTable[k=0]);
		for (j=1;j<m_nMakePalettEnum;j++) {
			if (mn>(mt=m_ColorSpace.DistanceGCS(&gcs2,&m_GcsTable[j]))) {
				mn=mt;
				k=j;
			}
		}
	}
	else {
		if (m_pQcube[i]==NULL) MakeCube(x,y,z);
		k=FindClosepaletteSub(m_pQcube[i],x,y,z);
	}
	return k;
}

//---------------------------------------------------------------------------

void XImageConverter::Mtscan(const XImage &m_pSorceImage, int width, int height)
// Anime2処理開始
{
	int x,y;
	int j,i,k,kk;
	int dx,dy,dz,gx,gy,gz;
	GCS gcs1;

	Anime2line a2l[32];


	char dit[16] = { 0, 8, 2,10, 12, 4,14, 6, 3,11, 1, 9,15, 7,13, 5 };
//	char dit[4] = {  0, 2 , 3, 1 };

	for (y=0;y<height;y++)
	{
		for (x=0;x<width;x++)
		{
			i = m_pSorceImage.GetPixel(x, y);
			if( i == m_nTransPal )
			{
				k=m_nTransPal;
			}
			else
			{

				//16dither
				dx=dy=dz=0;
				m_ColorSpace.ConvRGBtoGCS(&gcs1,i);
				for (j=0;j<18;j++)
				{
					gx=gcs1.X+dx;
					gy=gcs1.Y+dy;
					gz=gcs1.Z+dz;
					a2l[j].pal=kk=FindClosepalette(gx,gy,gz);
					a2l[j].d=	m_GcsTable[kk].Z;

					dx=(gx-m_GcsTable[kk].X)*m_nDitherLevel/10;
					dy=(gy-m_GcsTable[kk].Y)*m_nDitherLevel/10;
					dz=(gz-m_GcsTable[kk].Z)*m_nDitherLevel/10;
				}
				//距離ソート
				qsort(&a2l,16,sizeof(Anime2line),qsortanime);

				//dit位置とソート位置から決定
				k=a2l[dit[(y & 3)*4+(x & 3)]].pal;
			}
			m_pPalbuf[x+y*m_nPicx]=(unsigned char)k;

		}
//		if (theQuantSign.m_Terminated) return;
		CallProgressFunction(BARBR*y/height+BARBO);
	}

}

//---------------------------------------------------------------------------
/*
出典
	矩形画像走査の一般化とその応用
	永江 孝規
	平成 3 年 2 月 16 日
*/
#define even(_x_) (0==(_x_)% 2)
void XImageConverter::Link(const XImage &m_pSorceImage, int a, int b)
// gPeanos処理
{
	int x,y,i,dx,dy,dz,k,gx,gy,gz;
	GCS gcs1;

	x=(m_nGpsx+=a);
	y=(m_nGpsy+=b);
	//誤差含み書き出し

	i=  m_pSorceImage.GetPixel(x, y);
	if (i==m_nTrans)
	{
		k=m_nTransPal;
	}
	else
	{
		m_ColorSpace.ConvRGBtoGCS(&gcs1,i);

		dx=m_nGpsdx*m_nDitherLevel/10;
		dy=m_nGpsdy*m_nDitherLevel/10;
        dz=m_nGpsdz*m_nDitherLevel/10;

		if (dx>m_nMxdx)
			dx=m_nMxdx;
		else if (dx<-m_nMxdx)
			dx=-m_nMxdx;
		if (dy>m_nMxdy)
			dy=m_nMxdy;
		else if (dy<-m_nMxdy)
			dy=-m_nMxdy;
		if (dz>m_nMxdz)
			dz=m_nMxdz;
		else if (dz<-m_nMxdz)
			dz=-m_nMxdz;

		gx=gcs1.X+dx;
		gy=gcs1.Y+dy;
		gz=gcs1.Z+dz;
		k=FindClosepalette(gx,gy,gz);

       	m_nGpsdx=gx-m_GcsTable[k].X;
       	m_nGpsdy=gy-m_GcsTable[k].Y;
       	m_nGpsdz=gz-m_GcsTable[k].Z;
	}
	m_pPalbuf[x+y*m_nPicx]=(unsigned char)k;

    //progress set
	CallProgressFunction(BARBR*(m_nGpsc++)/(m_nPicx*m_nPicy)+BARBO);

}

void XImageConverter::Transpose(int *a, int *b, int *c, int *d, int *e, int *f)
{
	int t;

	t = *a; *a = *b; *b = t;
	t = *c; *c = *d; *d = t;
	t = *e; *e = *f; *f = t;
}

void XImageConverter::Bic(const XImage &m_pSorceImage, int m, int n, int a, int b, int c, int d)
{
	int	m1, m2;

	m1 = m2 = m / 2;
	if(even(n) && even(m1)){ m1++; m2--; } // 割り切れなかったときの調整
	Bit(m_pSorceImage, m1, n, a, b, c, d);
	Link(m_pSorceImage, a, b);
	Bit(m_pSorceImage, m2, n, a, b, - c, - d);
}

void XImageConverter::Bit(const XImage &m_pSorceImage, int m, int n, int a, int b, int c, int d)
{
	int	i;

	if(1 == m) for(i = 1; i < n; Link(m_pSorceImage, c, d), i++);
	else if(1 == n) for(i = 1; i < m; Link(m_pSorceImage, a, b), i++);
	else{
		int	m1, m2;

		if(m < n) Transpose(&m, &n, &a, &c, &b, &d);
		m1 = m2 = m / 2;
		switch(m % 4){	case 1: m2++; break;
				case 2: m1++; m2--; break;
				case 3: m1++; }
		Bic(m_pSorceImage, m1, n, a, b, c, d);
		Link(m_pSorceImage, a, b);
		Bit(m_pSorceImage, m2, n, a, b, c, d);
	}
}

void XImageConverter::Biscan(const XImage &m_pSorceImage, int width, int height)
// gPeanos処理開始
{
	m_nGpsx=m_nGpsy=m_nGpsc=0;

    m_nGpsdx=m_nGpsdy=m_nGpsdz=0;

    Link(m_pSorceImage, 0,0);
	if(even(width) && even(height)){
		if(height > width) Bic(m_pSorceImage, height, width, 0, 1, 1, 0);
		else		   Bic(m_pSorceImage, width, height, 1, 0, 0, 1);
	}
	else			   Bit(m_pSorceImage, width, height, 1, 0, 0, 1);
}


int XImageConverter::Execute(const XImage &m_pSorceImage, XImage &dst)
{
	/*
		入力はメンバのみ。事前に登録されていること。
		出力はGlobalAllocのDestinationDIBのみ。その他は随時確保、終了時破棄。
	*/
	int        i;
	int        colorcount;
	int        *tmpbuf;
	GCS        *dt[3];
	BOOL        pictureconvertfinish=FALSE;
	PALSTR      pal[256];
	double      wrapmx[13],wrapmn[13];
	double      wrapsign[13][3]= {
		{  1, 0, 0 },
		{  0, 1, 0 },
		{  0, 0, 1 },
		{  1, 1, 0 },
		{  1, 0, 1 },
		{  1, 0,-1 },
		{  1,-1, 0 },
		{  0, 1, 1 },
		{  0, 1,-1 },
		{  1, 1, 1 },
		{  1,-1, 1 },
		{  1,-1,-1 },
		{  1, 1,-1 }
	};


	// カラーBitsが24以下の場合は、DESTにコピーを作成するのみ
	if( m_pSorceImage.GetBitCount() < 24 )
	{
		dst = m_pSorceImage;
		return (0);
	}



	tmpbuf=NULL;
	m_pMedbuf=NULL;
	m_pSmedbuf=NULL;

	for (i=0;i<256;i++)
		pal[i].z = INT_MAX;


	// ソースイメージの幅
	m_nPicx = m_pSorceImage.GetWidth();
	// ソースイメージの高さ
	m_nPicy = m_pSorceImage.GetHeight();


	CallProgressFunction(BARCR/4+BARCO);

	for (i=0;i<CUBEALL;i++)
		m_pQcube[i]=NULL;
	dt[0] = dt[1] = dt[2] = NULL;

	try
	{

	{
		m_pPalbuf = (unsigned char*)malloc( m_nPicx * m_nPicy * sizeof(unsigned char));
	}
#ifdef _DEBUG
{
	makecrctable();
//	cg.m_CRC[cg.m_CRCnum++]=crc( m_nPicx * m_nPicy *sizeof(int),(unsigned char*) m_pOrgBuf);
}
#endif


	m_pPicBuf=(int*)malloc( m_nPicx * m_nPicy * sizeof(int) ); //共有パレット未対応

	m_nMakePalettEnum = m_nIndexVolume;
	if( m_bAdd16Pallete == TRUE )
		m_nMakePalettEnum -= 16;

	if( m_nTransparencyColor >= 0 )
	{
		m_nMakePalettEnum--;
		switch( m_nTransparencyColor )
		{
			case 0:
				m_nTrans = m_pSorceImage.GetPixel(0,0);
				break;
			case 1:
				m_nTrans = m_pSorceImage.GetPixel(0,m_nPicy);
				break;
			case 2:
				m_nTrans = m_pSorceImage.GetPixel(m_nPicx,0);
				break;
			default:
				m_nTrans = m_pSorceImage.GetPixel(m_nPicx,m_nPicy);
				break;
		}
	}
	else
	{
		m_nTrans = 0x01000000;
	}


	// テーブル作製
	m_ColorSpace.Create(m_nCbMag,m_nCrMag,m_nCrMag,m_nPalettePrecision);

	{
		long adp=0;
	   	int m,r,rgb,t[256];
		int loop,lp;
		int k,i,j,c,ct;

		loop = 1;

		m = 0xff & ~m_nPictureMask;
	   	k = (m_nPictureMask+1)/2;
		for (i=0;i<256;i++)
		{
			r = i+k;
			if (r>255)
				r = 255;
			r &= m;
			t[i] = r;
		}

	   	for ( i=0; i < m_nMakePalettEnum; i++ )
			pal[i].c = 0;
		for (lp=0;lp<loop;lp++)
		{
#define nEDGE 0
//------------------------------------------------------------------------------
#if nEDGE==0
	// 輪郭
#define colortrim1(n) (unsigned char)(max(0.,min(255.,n)))
			{

				int sobel1[9]={
					 1, 0,-1,
					 2, 0,-2,
					 1, 0,-1};
				int sobel2[9]={
					 1, 2, 1,
					 0, 0, 0,
					-1,-2,-1};

				unsigned char *tmpbuf;
				int r,g,b,rm,gm,bm;
				int rx,gx,bx;
				int c;
				int dx,dy,x,y;
				int rmx,rmn,gmx,gmn,bmx,bmn;

				int ry,gy,by;
				int sa;

				tmpbuf=(unsigned char *)malloc( m_nPicx * m_nPicy *6*sizeof(unsigned char));
				memset(tmpbuf,0, m_nPicx * m_nPicy *6*sizeof(unsigned char));

				for (y=0;y< m_nPicy ;y++)
				{
					for (x=0;x< m_nPicx ;x++)
					{
						if (x>0 && x< m_nPicx -1 && y>0 && y< m_nPicy -1)
						{
							rx=0,gx=0,bx=0;
							ry=0,gy=0,by=0;
							rmx=0,gmx=0,bmx=0;
							rmn=1000,gmn=1000,bmn=1000;

							c = m_pSorceImage.GetPixel(x, y);
							rm=(c)     & 0xff;
							gm=(c>>8)  & 0xff;
							bm=(c>>16) & 0xff;

							sa=0;
							for (dy=-1;dy<2;dy++)
							{
								for (dx=-1;dx<2;dx++)
								{
									c = m_pSorceImage.GetPixel((x+dx), (y+dy));
									r=(c)     & 0xff;
									g=(c>>8)  & 0xff;
									b=(c>>16) & 0xff;
									rx+=sobel1[sa]*(r-rm);
									gx+=sobel1[sa]*(g-gm);
									bx+=sobel1[sa]*(b-bm);
									ry+=sobel2[sa]*(r-rm);
									gy+=sobel2[sa]*(g-gm);
									by+=sobel2[sa]*(b-bm);

									rmx=max(r,rmx);
									rmn=min(r,rmn);
									gmx=max(g,gmx);
									gmn=min(g,gmn);
									bmx=max(b,bmx);
									bmn=min(b,bmn);

									sa++;
								}
							}
							tmpbuf[(x+y* m_nPicx )*6+0]=colortrim1(128.-(double)rx/max(rmx-rmn,8)*64.);
							tmpbuf[(x+y* m_nPicx )*6+1]=colortrim1(128.-(double)ry/max(rmx-rmn,8)*64.);
							tmpbuf[(x+y* m_nPicx )*6+2]=colortrim1(128.-(double)gx/max(gmx-gmn,8)*64.);
							tmpbuf[(x+y* m_nPicx )*6+3]=colortrim1(128.-(double)gy/max(gmx-gmn,8)*64.);
							tmpbuf[(x+y* m_nPicx )*6+4]=colortrim1(128.-(double)bx/max(bmx-bmn,8)*64.);
							tmpbuf[(x+y* m_nPicx )*6+5]=colortrim1(128.-(double)by/max(bmx-bmn,8)*64.);
						}

					}

					CallProgressFunction(BARDR*y/ m_nPicy +BARDO);	
				}
				{
					double r,g,b;
					int rx,ry,gx,gy,bx,by;
					int rxm,rym,gxm,gym,bxm,bym;
					int mm;

					int ta=0,tb=0;

					for (y=0;y< m_nPicy ;y++)
					{
						for (x=0;x< m_nPicx ;x++)
						{
							if (x>0 && x< m_nPicx -1 && y>0 && y< m_nPicy -1)
							{
								rxm=tmpbuf[(x+y* m_nPicx )*6+0];
								rym=tmpbuf[(x+y* m_nPicx )*6+1];
								gxm=tmpbuf[(x+y* m_nPicx )*6+2];
								gym=tmpbuf[(x+y* m_nPicx )*6+3];
								bxm=tmpbuf[(x+y* m_nPicx )*6+4];
								bym=tmpbuf[(x+y* m_nPicx )*6+5];

								r=0, g=0, b=0;

								for (dy=-1;dy<2;dy++)
								{
									for (dx=-1;dx<2;dx++)
									{
											rx=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+0];
											ry=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+1];
											gx=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+2];
											gy=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+3];
											bx=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+4];
											by=tmpbuf[((x+dx)+(y+dy)* m_nPicx )*6+5];

											r+=sqrt((rx-rxm)*(rx-rxm)+(ry-rym)*(ry-rym));
											g+=sqrt((gx-gxm)*(gx-gxm)+(gy-gym)*(gy-gym));
											b+=sqrt((bx-bxm)*(bx-bxm)+(by-bym)*(by-bym));
									}
								}
								mm=colortrim1(255-sqrt(r*r+g*g+b*b)/8);
								mm=(mm<160)?0:255;
							}
							else mm=0;

							if ((rgb= m_pSorceImage.GetPixel( x, y ) )!= m_nTrans )
							{
								if (mm==0)
								{
									 m_pPicBuf [x+y* m_nPicx +adp]=t[rgb & 0xff] | (t[(rgb & 0xff00)>>8]<<8) | (t[(rgb & 0xff0000)>>16]<<16);
									ta++;
								}
								else
								{
									 m_pPicBuf [x+y* m_nPicx +adp]=t[rgb & 0xff] | (t[(rgb & 0xff00)>>8]<<8) | (t[(rgb & 0xff0000)>>16]<<16) | 0x01000000;
									tb++;
								}
							}
							else
							{
								 m_pPicBuf [x+y* m_nPicx +adp]=0x40000000;
							}
						}
						CallProgressFunction(BARER*y/ m_nPicy +BAREO);
					}
					if(tmpbuf)
						free(tmpbuf), tmpbuf=NULL;
				}
			}
#endif
//------------------------------------------------------------------------------
#if nEDGE==1
			int xx, yy;
			j=i= m_nPicx * m_nPicy ;
			// 出現色とその回数
			for (yy = 0; yy < m_nPicy ; yy++)
			{
				for (xx = 0; xx < m_nPicx ;xx++)
				{
					if ((rgb= m_pSorceImage.GetPixel(xx, yy) )!= m_nTrans )
						 m_pPicBuf[ (xx + yy * m_nPicx) + adp]=t[rgb & 0xff] | (t[(rgb & 0xff00)>>8]<<8) | (t[(rgb & 0xff0000)>>16]<<16);
					else
						 m_pPicBuf[ (xx + yy * m_nPicx) + adp]=0x40000000;
				}
			}
#endif
//------------------------------------------------------------------------------		
			adp+= m_nPicx * m_nPicy ;
		}
		tmpbuf=(int*)malloc(adp*2*sizeof(int));

		array_hsort( m_pPicBuf ,adp);

		c= m_pPicBuf [0] & 0xffffff;
		ct=1;
		colorcount=0;
		for (i=1;i<adp;i++)
		{
			if (c!=(k= m_pPicBuf [i]))
			{
				tmpbuf[colorcount*2]=ct;
				tmpbuf[(colorcount++)*2+1]=c;
				c=k & 0xffffff;
				ct=0;
			}
			if ((k & 0x1000000)>0)
				ct+=8;
			else
				ct+=1;
		}
		if (c!=0x40000000)
		{
			tmpbuf[colorcount*2]=ct;
			tmpbuf[(colorcount++)*2+1]=c;
		}

		CallProgressFunction(BARFR/2+BARFO);

		// median cut用空間を作製
		m_pMedbuf=  (MED*) malloc(colorcount*sizeof(MED ));
		m_pSmedbuf= (MED**)malloc(colorcount*sizeof(MED*));
		for (i=0;i<colorcount;i++)
		{
			j=tmpbuf[i*2+1];
			m_pMedbuf[i].cnt=tmpbuf[i*2];
			m_ColorSpace.ConvRGBtoGCS(&(m_pMedbuf[i].gcs),j);

			m_pSmedbuf[i]=&m_pMedbuf[i];
		}
		if(tmpbuf)
			free(tmpbuf),tmpbuf=NULL;
	}

	CallProgressFunction(BARFR+BARFO);



	// median cut ////////////////////////////////////////////////////////////////
	{
		int i,j,k,mxa,ct,cl,f,makepal;
		double mx;

		m_DivideBox.MakeMEDbox(m_Medbox,m_pSmedbuf,0,0,colorcount,&m_ColorSpace);

		if ( m_nMakePalettEnum )
			CallProgressFunction(BARAR*1/m_nMakePalettEnum+BARAO);
		i=1;
		makepal=0;
		while( makepal < m_nMakePalettEnum && i < 760 )
		{
//			_RPT1(_CRT_WARN,"%d:",makepal);

			mx= m_Medbox [mxa=0].strain;
			for (j=1;j<i;j++)
			{
				if (mx< m_Medbox [j].strain)
				{
					mxa=j;
					mx= m_Medbox [mxa].strain;
				}
			}

			if (mx>0)
			{
            	if ( m_DivideBox.divideBox( m_Medbox ,m_pSmedbuf,mxa,i,&m_ColorSpace) != true )
					continue;
            }
			else
			{
				break;
			}
			i++;

			//パレットソート

			//有効パレット数カウント
			ct=0;
		   	for (j=0;j<i;j++)
			{
				cl= m_Medbox [j].rgb;
				f=TRUE;
				for (k=0;k<ct;k++)
					if (pal[k].c==cl)
						f = false;

				if(f==TRUE)
				{
					GCS a;
					m_ColorSpace.ConvRGBtoGCS(&a,cl);
					pal[ct].c=cl;
					pal[ct].z=a.Z;
					ct++;
				}
			}
			makepal=ct;

			CallProgressFunction((int)(BARAR*( ( (1-1./sqrt(max(makepal,1))) /2) + ((double)makepal/m_nMakePalettEnum/2.) )+BARAO));

		}

	}

	if (m_pMedbuf)
		free(m_pMedbuf),m_pMedbuf=NULL;
	if (m_pSmedbuf)
		free(m_pSmedbuf),m_pSmedbuf=NULL;

//--------------

	{
		int i,m;
		GCS a;
		int p[16]={
			0x000000,
			0x000080,
			0x008000,
			0x008080,
			0x800000,
			0x800080,
			0x808000,
			0x808080,
			0xc0c0c0,
			0x0000ff,
			0x00ff00,
			0x00ffff,
			0xff0000,
			0xff00ff,
			0xffff00,
			0xffffff };

		m=m_nMakePalettEnum;
		if ( m_bAdd16Pallete ==TRUE )
		{
			for (i=0;i<16;i++)
			{
				pal[m+i].c = p[i];
				m_ColorSpace.ConvRGBtoGCS(&a,p[i]);
				pal[m+i].z = a.Z;
			}
			m+=16;
		}

		qsort(pal,m,sizeof(PALSTR),qsortpal);
	}

	// パレットリストを作製
	{
		int i,j,m,x,y,z;
		double tx=0,ty=0,tz=0;
		double d;

		m=m_nMakePalettEnum;
		if ( m_bAdd16Pallete ==TRUE)
			m +=16;
		for (i=0;i<m;i++)
		{
			{
				j = pal[i].c;
			}
			m_ColorSpace.ConvRGBtoGCS(&m_GcsTable[i],j);

			tx += x = m_GcsTable[i].X;
			ty += y = m_GcsTable[i].Y;
			tz += z = m_GcsTable[i].Z;

			for (j=0;j<13;j++)
			{
				d = x * wrapsign[j][0] + y * wrapsign[j][1] + z * wrapsign[j][2];
				if (i==0)
					wrapmx[j] = wrapmn[j] = d;
				else {
					if (d<wrapmn[j])
						wrapmn[j] = d;
					if (d>wrapmx[j])
						wrapmx[j] = d;
				}
			}

		}



		m_nMxdx = (int)(wrapmx[0]-wrapmn[0]);
		m_nMxdy = (int)(wrapmx[1]-wrapmn[1]);
		m_nMxdz = (int)(wrapmx[2]-wrapmn[2]);
	}
	m_nMakePalettEnum= m_nIndexVolume ;
	if ( m_nTransparencyColor >=0)
	{
		m_nTransPal = --m_nMakePalettEnum;
		pal[m_nMakePalettEnum].c =  m_nTrans ;
	}

	// convert graphics
	{
		int loop,lp;


		loop=1;
		for (lp=0;lp<loop;lp++)
		{
			if ( m_nDitherMode ==0)
			{
				GCS gcs1;
				int i,x,y,k;

				for (y=0;y< m_nPicy ;y++)
				{
					for (x=0;x< m_nPicx ;x++)
					{
						i = m_pSorceImage.GetPixel(x, y);
						if (i== m_nTrans )
						{
							k=m_nTransPal;
						}
						else
						{
							m_ColorSpace.ConvRGBtoGCS(&gcs1,i);
							k=FindClosepalette(gcs1.X,gcs1.Y,gcs1.Z);
						}
						m_pPalbuf[x+y* m_nPicx ]=(unsigned char)k;
					}

					CallProgressFunction(BARBR*y/ m_nPicy +BARBO);
				}
			}
			else if ( m_nDitherMode ==2 ||  m_nDitherMode ==3)
			{
				GCS gcs1;
				GCS *dttmp;
				int i,x,y,k,gx,gy,gz,m_DivideBox,dvs,dx,dy,dz,sx,sy,m,kk;

				for (i=0;i<3;i++)
				{
					dt[i]=(GCS*)malloc(( m_nPicx +4)*sizeof(GCS));
					memset(dt[i],0,sizeof(GCS)*( m_nPicx +4));
				}

				m_DivideBox=1, dvs=-1;

				for (y=0;y< m_nPicy ;y++)
				{
					for (x=((m_DivideBox==1)?0: m_nPicx -1);x< m_nPicx  && x>=0;x+=m_DivideBox)
					{
						i= m_pSorceImage.GetPixel(x, y);
						if (i== m_nTrans )
						{
							k=m_nTransPal;
						}
						else
						{
							m_ColorSpace.ConvRGBtoGCS(&gcs1,i);

							dx=dy=dz=0;
							if ( m_nDitherMode ==2)
							{
								//FS
								for (sx=-1;sx<2;sx++)
								{
									for (sy=0;sy<2;sy++)
									{
										if ( (m = m_fbpattern[1-sy][sx+1]) > 0)
										{
											dx += dt[sy][kk = x + 2 + sx * m_DivideBox].X*m;
											dy += dt[sy][kk].Y * m;
											dz += dt[sy][kk].Z * m;
										}
									}
								}
								dx/=160, dy/=160, dz/=160;
							}
							else
							{
								//maem
								for (sx=-2;sx<3;sx++)
								{
									for (sy=0;sy<3;sy++)
									{
										if ((m= m_maempattern[2-sy][sx+2])>0)
										{
											dx+=dt[sy][kk=x+2+sx*m_DivideBox].X*m;
											dy+=dt[sy][kk].Y*m;
											dz+=dt[sy][kk].Z*m;
										}
									}
								}
                                dx/=480, dy/=480, dz/=480;
                            }

							if (dx>m_nMxdx)
								dx=m_nMxdx;
							else if (dx<-m_nMxdx)
								dx=-m_nMxdx;
							if (dy>m_nMxdy)
								dy=m_nMxdy;
							else if (dy<-m_nMxdy)
								dy=-m_nMxdy;
							if (dz>m_nMxdz)
								dz=m_nMxdz;
							else if (dz<-m_nMxdz)
								dz=-m_nMxdz;


							gx = gcs1.X + dx;
							gy = gcs1.Y + dy;
							gz = gcs1.Z + dz;
							k = FindClosepalette(gx,gy,gz);
           					dt[0][x+2].X = (gx-m_GcsTable[k].X) * m_nDitherLevel;
							dt[0][x+2].Y = (gy-m_GcsTable[k].Y) * m_nDitherLevel;
							dt[0][x+2].Z = (gz-m_GcsTable[k].Z) * m_nDitherLevel;
						}

						m_pPalbuf[x+y* m_nPicx ]=(unsigned char)k;
					}
					dttmp=dt[2]; dt[2]=dt[1]; dt[1]=dt[0]; dt[0]=dttmp;
	                memset(dt[0],0,sizeof(GCS)*( m_nPicx +4));

					CallProgressFunction(BARBR*y/ m_nPicy +BARBO);
					m_DivideBox=m_DivideBox*dvs;
				}
			}
            else if ( m_nDitherMode ==1)
			{
                Biscan(m_pSorceImage, m_nPicx , m_nPicy );
            }
			else if ( m_nDitherMode ==4)
			{
				Mtscan(m_pSorceImage, m_nPicx , m_nPicy );
			}


			// 修正入り
			{
				int i;
				unsigned int  nBitsCount;
				if ( m_nIndexVolume <3)
					nBitsCount = 1;
				else if ( m_nIndexVolume <17)
					nBitsCount = 4;
				else
					nBitsCount = 8;
	
				
				dst.SetSize(m_nIndexVolume, nBitsCount, m_pSorceImage.GetWidth(), m_pSorceImage.GetHeight());
				for(i=0;i< m_nIndexVolume ;i++)
				{
					dst.GetColorMap(i).red   = XImgTools::_GetRvalue(pal[i].c);
					dst.GetColorMap(i).green = XImgTools::_GetGvalue(pal[i].c);
					dst.GetColorMap(i).blue  = XImgTools::_GetBvalue(pal[i].c);
				}
			}
		}//loop
	}

	pictureconvertfinish=TRUE;


	if (pictureconvertfinish)
	{
		int x,y;
		for (y=0;y< m_nPicy ;y++)
		{
			for (x=0;x< m_nPicx ;x++)
			{
				 dst.SetPixelNo(x,y ,m_pPalbuf[ x + y * m_nPicx ]);
			}
		}
	}

	}
	catch( XImageException *e )
	{
		if(tmpbuf)
			free(tmpbuf);
		if(m_pMedbuf)
			free(m_pMedbuf);
		if(m_pSmedbuf)
			free(m_pSmedbuf);
		m_pSmedbuf=NULL;
		if( m_pPicBuf )
			free( m_pPicBuf );
		if(m_pPalbuf)
			free(m_pPalbuf);
		for(i=0;i<CUBEALL;i++)
		{
			if(m_pQcube[i])
			{
				free(m_pQcube[i]);
			}
		}
		for(i=0;i<3;i++)
		{
			if(dt[i])
			{
				free(dt[i]);
			}
		}
		throw e;
	}
	if(tmpbuf)
		free(tmpbuf);
	if(m_pMedbuf)
		free(m_pMedbuf);
	if(m_pSmedbuf)
		free(m_pSmedbuf);
	m_pSmedbuf=NULL;
	if( m_pPicBuf )
		free( m_pPicBuf );
	if(m_pPalbuf)
		free(m_pPalbuf);
	for(i=0;i<CUBEALL;i++)
	{
		if(m_pQcube[i])
		{
			free(m_pQcube[i]);
		}
	}
	for(i=0;i<3;i++)
	{
		if(dt[i])
		{
			free(dt[i]);
		}
	}
	return (0);
}
void XImageConverter::CallProgressFunction(unsigned int progress)
{
	if(m_ProgressFunction == 0)
		return ;
	bool abort = false ;
	unsigned int percent = progress ;
	if(percent > 100)
		percent = 100 ;
	if(m_ProgressFunction != 0)
	{
		m_ProgressFunction(*this,
			   m_pProgressData,
			   0,
			   0,
			   percent,
			   abort);
	}
	if( abort )
		XImageException::Throw( XIMG_ERR_PROGRESS_CANCEL );
	return ;
}


