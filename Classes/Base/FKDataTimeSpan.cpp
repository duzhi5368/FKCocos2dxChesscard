#include "FKDataTimeSpan.h"
//------------------------------------------------------------
CFKDateTimeSpan::CFKDateTimeSpan()
	: m_dblspan( 0.0 )
{

}
//------------------------------------------------------------
CFKDateTimeSpan::CFKDateTimeSpan(const CFKDateTimeSpan& span)
	: m_dblspan( span.m_dblspan )
{

}
//------------------------------------------------------------
CFKDateTimeSpan::CFKDateTimeSpan(double dblspan)
	: m_dblspan( dblspan )
{

}
//------------------------------------------------------------
CFKDateTimeSpan::CFKDateTimeSpan(int lDays, int nHours, int nMins,
	int nSecs, int nMillisecs)
	: m_dblspan(lDays + (double)nHours / 24.0 + (double)nMins / 1440.0 +
	(double)nSecs / 86400.0 + (double)nMillisecs / 86400000.0)
{

}
//------------------------------------------------------------
int CFKDateTimeSpan::GetDays()
{
	return (int)m_dblspan;
}
//------------------------------------------------------------
int CFKDateTimeSpan::GetHours()
{
	return (int)((m_dblspan - floor(m_dblspan)) * 24.0);
}
//------------------------------------------------------------
int CFKDateTimeSpan::GetMinutes()
{
	double dhours = m_dblspan * 24.0;
	return (int)((dhours - floor(dhours)) * 60.0);
}
//------------------------------------------------------------
int CFKDateTimeSpan::GetSeconds()
{
	double dmins = m_dblspan * 1440.0;
	return (int)((dmins - floor(dmins)) * 60.0);
}
//------------------------------------------------------------
int CFKDateTimeSpan::GetMilliseconds()
{
	double dsecs = m_dblspan * 86400.0;
	return (int)((dsecs - floor(dsecs)) * 1000.0);
}
//------------------------------------------------------------
double CFKDateTimeSpan::GetTotalDays()
{
	return floor(m_dblspan + 0.5);
}
//------------------------------------------------------------
double CFKDateTimeSpan::GetTotalHours()
{
	return floor(m_dblspan * 24.0 + 0.5);
}
//------------------------------------------------------------
double CFKDateTimeSpan::GetTotalMinutes()
{
	return floor(m_dblspan * 1440.0 + 0.5);
}
//------------------------------------------------------------
double CFKDateTimeSpan::GetTotalSeconds()
{
	return floor(m_dblspan * 86400.0 + 0.5);
}
//------------------------------------------------------------
double CFKDateTimeSpan::GetTotalMilliseconds()
{
	return floor(m_dblspan * 86400000.0 + 0.5);
}
//------------------------------------------------------------
CFKDateTimeSpan::operator double() const
{
	return m_dblspan;
}
//------------------------------------------------------------
CFKDateTimeSpan& CFKDateTimeSpan::operator =(double dblsrc)
{
	m_dblspan = dblsrc;
	return *this;
}
//------------------------------------------------------------
CFKDateTimeSpan& CFKDateTimeSpan::operator =(const CFKDateTimeSpan& span)
{
	m_dblspan = span.m_dblspan;
	return *this;
}
//------------------------------------------------------------
CFKDateTimeSpan	 CFKDateTimeSpan::operator +(const CFKDateTimeSpan& span)
{
	CFKDateTimeSpan _span(m_dblspan + span.m_dblspan);
	return CFKDateTimeSpan(_span);
}
//------------------------------------------------------------
CFKDateTimeSpan	 CFKDateTimeSpan::operator -(const CFKDateTimeSpan& span)
{
	CFKDateTimeSpan _span(m_dblspan - span.m_dblspan);
	return CFKDateTimeSpan(_span);
}
//------------------------------------------------------------
CFKDateTimeSpan& CFKDateTimeSpan::operator +=(const CFKDateTimeSpan& span)
{
	m_dblspan += span.m_dblspan;
	return *this;
}
//------------------------------------------------------------
CFKDateTimeSpan& CFKDateTimeSpan::operator -=(const CFKDateTimeSpan& span)
{
	m_dblspan -= span.m_dblspan;
	return *this;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator ==(const CFKDateTimeSpan& span)
{
	return m_dblspan == span.m_dblspan;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator !=(const CFKDateTimeSpan& span)
{
	return m_dblspan != span.m_dblspan;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator <(const CFKDateTimeSpan& span)
{
	return m_dblspan < span.m_dblspan;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator >(const CFKDateTimeSpan& span)
{
	return m_dblspan > span.m_dblspan;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator <=(const CFKDateTimeSpan& span)
{
	return m_dblspan <= span.m_dblspan;
}
//------------------------------------------------------------
bool CFKDateTimeSpan::operator >=(const CFKDateTimeSpan& span)
{
	return m_dblspan >= span.m_dblspan;
}
//------------------------------------------------------------