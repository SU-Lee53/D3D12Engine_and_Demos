#pragma once

inline std::string HrToString(HRESULT hr)
{
    std::string output = std::format("HRESULT of 0x{}\n", static_cast<UINT>(hr));
    OutputDebugStringA(output.c_str());

    return output;
}

class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
    HRESULT Error() const { return m_hr; }
private:
    const HRESULT m_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}
inline BOOL FalseIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        return FALSE;
    }

    return TRUE;
}
