
//  $Id: front_end.C,v 1.19 2009/10/30 01:59:08 stanchen Exp $


#include "front_end.H"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** Module for doing windowing. **/
void FrontEnd::do_window(const matrix<double>& inFeats,
    matrix<double>& outFeats) const
    {
    //  Get parameters.
    //  Input samples per second.
    double sampleRate = get_float_param(m_params, "window.sample_rate",
        20000.0);
    //  Output frames per second.
    double framesPerSec = get_float_param(m_params, "window.frames_per_sec",
        100.0);
    //  Width of each window, in seconds.
    double windowWidth = get_float_param(m_params, "window.window_size",
        0.025);
    //  Whether to do Hamming or rectangular windowing.
    bool doHamming = get_bool_param(m_params, "window.hamming", true);

    //  Get number of input samples.
    int inSampCnt = inFeats.size1();
    if (inFeats.size2() != 1)
        throw runtime_error("Windowing expected vector input.");

    //  Input sampling period in seconds.
    double samplePeriod = 1.0 / sampleRate;
    //  Output frame period, in seconds.
    double framePeriod = 1.0 / framesPerSec;
    //  Number of samples per window.
    int sampPerWindow = (int) (windowWidth / samplePeriod + 0.5);
    //  Number of samples to shift between each window.
    int sampShift = (int) (framePeriod / samplePeriod + 0.5);
    //  Number of output frames.
    int outFrameCnt = (inSampCnt - sampPerWindow) / sampShift + 1;

    //  Allocate output matrix and fill with zeros.
    outFeats.resize(outFrameCnt, sampPerWindow);
    outFeats.clear();

    //  BEGIN_LAB
    //
    //  Input:
    //      "inFeats", a matrix containing a single column holding the
    //      input samples for an utterance.  Each row holds a single sample.
    //
    //      inFeats(0 .. (inSampCnt - 1), 0)
    //
    //  Output:
    //      "outFeats", which should contain the result of windowing.
    //
    //      outFeats(0 .. (outFrameCnt - 1), 0 .. (sampPerWindow - 1))
    //
    //      Each row corresponds to a frame, and should hold the
    //      windowed samples for that frame.
    //      It has already been allocated to be of the correct size.
    //      If the boolean "doHamming" is true, then a Hamming
    //      window should be applied; otherwise, a rectangular
    //      window should be used.
    //
    //  See "inSampCnt", "sampPerWindow", "sampShift", and "outFrameCnt"
    //  above for quantities you will need for this computation.
    //
    //  When accessing matrices such as "inFeats" and "outFeats",
    //  use a syntax like "inFeats(frmIdx, dimIdx)" to access elements;
    //  using square brackets as in normal C arrays won't work.
    
    //  Build the window function, if needed.
    vector<double> arrHam(sampPerWindow);
    if (doHamming)
        {
        for (int sampIdx = 0; sampIdx < sampPerWindow; ++sampIdx)
            arrHam[sampIdx] = 0.54 -
                0.46 * cos(2 * M_PI * sampIdx / (sampPerWindow - 1));
        }

    //  Do the computation.
    for (int frmIdx = 0, curSamp = 0; frmIdx < outFrameCnt; ++frmIdx,
        curSamp += sampShift)
        {
        for (int sampIdx = 0; sampIdx < sampPerWindow; ++sampIdx)
            outFeats(frmIdx, sampIdx) = inFeats(curSamp + sampIdx, 0);
        if (doHamming)
            {
            for (int sampIdx = 0; sampIdx < sampPerWindow; ++sampIdx)
                outFeats(frmIdx, sampIdx) *= arrHam[sampIdx];
            }
        }

    //  END_LAB
    }

/** Module for doing FFT. **/
void FrontEnd::do_fft(const matrix<double>& inFeats,
    matrix<double>& outFeats) const
    {
    //  Make output dimension the smallest power of 2 at least as
    //  large as input dimension.
    int inFrameCnt = inFeats.size1();
    int inDimCnt = inFeats.size2();
    int outDimCnt = 2;
    while (outDimCnt < inDimCnt)
        outDimCnt *= 2;

    //  Allocate output matrix and fill with zeros.
    outFeats.resize(inFrameCnt, outDimCnt);
    outFeats.clear();

    //  Input:
    //      "inFeats", a matrix with each row holding the windowed
    //      values for that frame.
    //
    //      inFeats(0 .. (inFrameCnt - 1), 0 .. (inDimCnt - 1))
    //
    //  Output:
    //      "outFeats", where an FFT should be applied to each
    //      row/frame of "inFeats".  
    //
    //      outFeats(0 .. (inFrameCnt - 1), 0 .. (outDimCnt - 1))
    //
    //      For a given row/frame "frmIdx", the real and imaginary
    //      parts of the FFT value for frequency i/(outDimCnt*T)
    //      where T is the sample period are held in
    //      outFeats(frmIdx, 2*i) and outFeats(frmIdx, 2*i+1),
    //      respectively.

    vector<double> fftBuf;
    for (int frmIdx = 0; frmIdx < inFrameCnt; ++frmIdx)
        {
        copy_matrix_row_to_vector(inFeats, frmIdx, fftBuf);
        //  Pad window with zeros, if needed.
        fftBuf.resize(outDimCnt, 0.0);
        real_fft(fftBuf);
        copy_vector_to_matrix_row(fftBuf, outFeats, frmIdx);
        }
    }

//  BEGIN_HIDE
/** Returns frequency @p freq mapped into Mel space. **/
static inline double get_mel_freq(double freq)
    {
    return 1127.0 * log(1.0 + freq / 700.0);
    }
//  END_HIDE

/** Module for mel binning. **/
void FrontEnd::do_melbin(const matrix<double>& inFeats,
    matrix<double>& outFeats) const
    {
    //  Number of mel bins to make.
    int numBins = get_int_param(m_params, "melbin.bins", 26);
    //  Whether to take log of output or not.
    bool doLog = get_bool_param(m_params, "melbin.log", true);
    //  Input samples per second.
    double sampleRate = get_float_param(m_params, "window.sample_rate",
        20000.0);
    double samplePeriod = 1.0 / sampleRate;

    //  Retrieve number of frames and dimension of input feature vectors.
    int inFrameCnt = inFeats.size1();
    int inDimCnt = inFeats.size2();
    int outDimCnt = numBins;

    //  Allocate output matrix and fill with zeros.
    outFeats.resize(inFrameCnt, outDimCnt);
    outFeats.clear();

    //  BEGIN_LAB
    //
    //  Input:
    //      "inFeats", holding the output of a real FFT.
    //
    //      inFeats(0 .. (inFrameCnt - 1), 0 .. (inDimCnt - 1))
    //
    //  Output:
    //      "outFeats", which should contain the result of
    //      mel-binning.
    //
    //      outFeats(0 .. (inFrameCnt - 1), 0 .. (outDimCnt - 1))
    //
    //      If the boolean "doLog" is true,
    //      then each value should be replaced with its natural
    //      logarithm, or 0 if its logarithm is negative.
    //      "outFeats" has been allocated to be of the correct size.
    //
    //  See "inFrameCnt", "inDimCnt", "outDimCnt", and "samplePeriod"
    //  above for quantities you will need for this computation.

    //  Basic unit in freqs output by FFT.
    double freqUnit = 1.0 / (samplePeriod * inDimCnt);

    //  Calc centers of mel bins.
    //  FFT contains re+im of only half the points.
    int fftPointCnt = inDimCnt / 2;
    double melFreqMax = get_mel_freq(fftPointCnt * freqUnit);
    vector<double> melFreqCenterList(outDimCnt + 1);
    for (int binIdx = 0; binIdx <= outDimCnt; ++binIdx)
        melFreqCenterList[binIdx] = melFreqMax * (binIdx + 1) /
            (outDimCnt + 1);

    //  Calc assignments to left bin.
    vector<int> binIdxList(fftPointCnt);
    vector<double> binWgtList(fftPointCnt);
    int binIdxCur = 0;
    //  Exclude freq=0.
    for (int inDimIdx = 1; inDimIdx < fftPointCnt; ++inDimIdx)
        {
        double melFreqCur = get_mel_freq(inDimIdx * freqUnit);
        //  Will be to left of center of right bin.
        while (melFreqCur > melFreqCenterList[binIdxCur])
            {
            ++binIdxCur;
            assert(binIdxCur <= outDimCnt);
            }
        binIdxList[inDimIdx] = binIdxCur - 1;

        //  Calc weight; triangular in Mel space.
        double melFreqLeftCenter = (binIdxCur > 0) ?
            melFreqCenterList[binIdxCur - 1] : 0.0;
        double melFreqRightCenter = melFreqCenterList[binIdxCur];
        binWgtList[inDimIdx] = (melFreqRightCenter - melFreqCur) /
            (melFreqRightCenter - melFreqLeftCenter);
        }

    //  Do the computation.
    for (int frmIdx = 0; frmIdx < inFrameCnt; ++frmIdx)
        {
        for (int inDimIdx = 1; inDimIdx < fftPointCnt; ++inDimIdx)
            {
            int binIdx = binIdxList[inDimIdx];
            double curWgt = binWgtList[inDimIdx];
            double realVal = inFeats(frmIdx, 2 * inDimIdx);
            double imagVal = inFeats(frmIdx, 2 * inDimIdx + 1);
            double powerVal = sqrt(realVal * realVal + imagVal * imagVal);
            if (binIdx >= 0)
                outFeats(frmIdx, binIdx) += powerVal * curWgt;
            if (binIdx + 1 < outDimCnt)
                outFeats(frmIdx, binIdx + 1) += powerVal * (1.0 - curWgt);
            }
        if (doLog)
            {
            for (int outDimIdx = 0; outDimIdx < outDimCnt; ++outDimIdx)
                {
                double& curValRef = outFeats(frmIdx, outDimIdx);
                curValRef = (curValRef > 1.0) ? log(curValRef) : 0.0;
                }
            }
        }

    //  END_LAB
    }

/** Module for doing discrete cosine transform. **/
void FrontEnd::do_dct(const matrix<double>& inFeats,
    matrix<double>& outFeats) const
    {
    //  Number of DCT coefficients to output.
    int numCoeffs = get_int_param(m_params, "dct.coeffs", 12);
    int inFrameCnt = inFeats.size1();
    int inDimCnt = inFeats.size2();
    int outDimCnt = numCoeffs;

    //  Allocate output matrix and fill with zeros.
    outFeats.resize(inFrameCnt, outDimCnt);
    outFeats.clear();

    //  BEGIN_LAB
    //
    //  Input:
    //      The matrix "inFeats", holding the output of mel-binning.
    //
    //      inFeats(0 .. (inFrameCnt - 1), 0 .. (inDimCnt - 1))
    //
    //  Output:
    //      The matrix "outFeats", which should contain the result of
    //      applying the DCT.
    //
    //      outFeats(0 .. (inFrameCnt - 1), 0 .. (outDimCnt - 1))
    //
    //      "outFeats" has been allocated to be of the correct size.
    //
    //  See "inFrameCnt", "inDimCnt", and "outDimCnt" above
    //  for quantities you will need for this computation.

    for (int frmIdx = 0; frmIdx < inFrameCnt; ++frmIdx)
        {
        for (int outDimIdx = 0; outDimIdx < outDimCnt; ++outDimIdx)
            {
            double prSum = 0.0;
            for (int inDimIdx = 0; inDimIdx < inDimCnt; ++inDimIdx)
                prSum += inFeats(frmIdx, inDimIdx) *
                    cos(M_PI * (outDimIdx + 1) *
                    (inDimIdx + 0.5) / inDimCnt);
            outFeats(frmIdx, outDimIdx) = sqrt(2.0 / inDimCnt) * prSum;
            }
        }

    //  END_LAB
    }

/** Main signal processing routine.
*   Calls each signal processing module in turn, unless
*   parameter says not to.
**/
void FrontEnd::get_feats(const matrix<double>& inAudio,
    matrix<double>& outFeats) const
    {
    if (get_bool_param(m_params, "frontend.null", false))
        {
        outFeats = inAudio;
        return;
        }
    matrix<double> curFeats(inAudio);
    if (get_bool_param(m_params, "frontend.window", true))
        {
        do_window(curFeats, outFeats);
        outFeats.swap(curFeats);
        }
    if (get_bool_param(m_params, "frontend.fft", true))
        {
        do_fft(curFeats, outFeats);
        outFeats.swap(curFeats);
        }
    if (get_bool_param(m_params, "frontend.melbin", true))
        {
        do_melbin(curFeats, outFeats);
        outFeats.swap(curFeats);
        }
    if (get_bool_param(m_params, "frontend.dct", true))
        {
        do_dct(curFeats, outFeats);
        outFeats.swap(curFeats);
        }
    outFeats.swap(curFeats);
    }


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


