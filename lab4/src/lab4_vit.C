
//  $Id: lab4_vit.C,v 1.56 2009/11/05 19:20:36 stanchen Exp stanchen $

#include <sys/times.h>
#include "lab4_vit.H"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Function for doing large-vocabulary Viterbi decoding.
 *
 *   @param graph Decoding graph to operate on.
 *   @param gmmProbs Matrix of logprobs for each GMM.
 *   @param outLabelList Return decoded words here.
 *   @param acousWgt The acoustic weight.
 *   @param beamLogProb Beam width as positive logprob base e, for beam
 *       pruning.
 *   @param beamStateCnt Max number active states (for rank pruning).  If 0,
 *       don't do any rank pruning.
 *   @param chart If non-empty, place diagnostic info here.
 *   @return Logprob of Viterbi path.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double do_viterbi(const Graph& graph, const matrix<double>& gmmProbs,
                  vector<int>& outLabelList, double acousWgt,
                  double beamLogProb, int beamStateCnt,
                  matrix<FrameCell>& chart) {
  //  Number of frames in utterance.
  int frmCnt = gmmProbs.size1();
  //  Number of states in graph.
  int stateCnt = graph.get_state_count();

  //  Objects for holding active states in cur frame and next frame.
  FrameData curFrame(stateCnt);
  FrameData nextFrame(stateCnt);

  //  Struct holding traceback word tree.
  WordTree wordTree;

  /** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
  *
  *   START OF SAMPLE CODE
  *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **

  //  Init logprob and traceback for start state at frame 0.
  FrameCell& startCell = curFrame.insert_cell(graph.get_start_state());
  startCell.assign(0.0, wordTree.get_root_node());

  //  Iterate over frames.
  for (int frmIdx = 0; frmIdx < frmCnt; ++frmIdx)
      {
      //  Active cells for frame "frmIdx" are held in "curFrame".
      //  Active cells for frame "frmIdx+1" are held in "nextFrame".

      //  Initialize "nextFrame" to be empty.
      nextFrame.clear();

      //  Compute pruning threshold here, if desired.

      //  Loop through active states in numeric order;
      //  assumes graph has been topologically sorted w.r.t.
      //  epsilon arcs.
      curFrame.reset_iteration();
      int curState;
      while (((curState = curFrame.get_next_state())) >= 0)
          {
          //  Find cell corresponding to "curState".
          //  Make copy here, because cells in FrameData object
          //  can move in memory if new cells are inserted.
          FrameCell curCell(curFrame.get_cell_by_state(curState));

          //  Pruning.
          //  if (curCell.get_log_prob() < threshLogProb)
          //      continue;

          //  Loop through arcs exiting current state.
          int arcCnt = graph.get_arc_count(curState);
          int arcId = graph.get_first_arc_id(curState);
          for (int arcIdx = 0; arcIdx < arcCnt; ++arcIdx)
              {
              Arc arc;
              arcId = graph.get_arc(arcId, arc);
              bool hasGmm = (arc.get_gmm() >= 0);
              int dstState = arc.get_dst_state();

              //  Fill in update of destination cell here.
              }
          }

      if (frmIdx < frmCnt)
          {
          //  Keep these lines; these copy info into a chart object
          //  so that it can be output for debugging purposes.
          if (chart.size1())
              copy_frame_to_chart(curFrame, frmIdx, chart);

          //  Swap last and cur frame data structures.
          curFrame.swap(nextFrame);
          }
      }

  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
  *
  *   END OF SAMPLE CODE
  *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  //
  //  BEGIN_LAB
  //
  //  Input:
  //      "frmCnt" = Number of frames in utterance.
  //      "stateCnt" = Number of states in graph.
  //      "graph" = The decoding graph/HMM, of type "Graph".
  //      "gmmProbs" = A matrix of doubles:
  //
  //          gmmProbs(0 .. (frmCnt - 1), 0 .. (#GMM's - 1))
  //
  //          that stores the log prob of each GMM for each frame.
  //      "acousWgt" = The acoustic weight.  GMM log probs should
  //          be multiplied by this factor before combining with
  //          other scores.
  //      "beamLogProb" = The pruning beam width, log base e,
  //          as a positive value.
  //      "beamStateCnt" = The max number of active states per frame,
  //          if doing rank pruning.  If 0, don't do any rank pruning.
  //
  //      "curFrame" = Object of type "FrameData", for storing cells
  //          for active states at current frame.
  //      "nextFrame" = Object of type "FrameData", for storing cells
  //          for active states at next frame.
  //      "wordTree" = Object of type "WordTree", for storing traceback
  //          word tree.
  //
  //  Output:
  //      The objects "curFrame", "nextFrame", and "wordTree" must
  //      be updated correctly.
  //
  //      In particular, you must correctly create/update cells
  //      for each active state at each frame.  Here is an example
  //      of the syntax for creating/updating a cell:
  //
  //      FrameCell& dstCell = nextFrame.insert_cell(dstStateIdx);
  //      dstCell.assign(dstLogProb, dstWordTreeIdx);
  //
  //      To access the attributes of an existing cell "curCell",
  //      you can use the methods "curCell.get_log_prob()" and
  //      "curCell.get_node_index()".
  //
  //      For each cell, you will need to set the Viterbi log prob
  //      (just as in Lab 2; don't forget the acoustic weight or
  //      the arc log prob).  For each cell, you will also need to set the
  //      index of the node in "wordTree" corresponding to the word
  //      sequence labeling the best path to the cell.
  //      For this, you will need to sometimes create nodes in "wordTree".
  //      Here is an example call for finding/creating a
  //      node in "wordTree":
  //
  //      int dstWordTreeIdx = wordTree.insert_node(srcWordTreeIdx, wordIdx);
  //
  //      Notes:
  //      - You need to worry about skip arcs in this lab.
  //      If arc.get_gmm() < 0 for an arc, it's a skip arc, i.e.,
  //      there is no GMM associated with this arc and it doesn't
  //      consume a frame.
  //      - "g_zeroLogProb" is a large negative number you can use
  //      to represent "ln 0" instead of the actual value of negative
  //      infinity.
  //      - Log probabilities are all base e, i.e., natural
  //      logarithms.
  //
  //  Fill in the Viterbi algorithm here.  Use the sample code above
  //  as a starting point.
  //
  //  The code for calculating the final probability and
  //  the best path is provided for you below.

  

  //
  //  END_LAB
  //

  //  Save last frame in chart object for debugging purposes.
  if (chart.size1()) copy_frame_to_chart(curFrame, frmCnt, chart);

  //  The code for calculating the final probability and
  //  the best word sequence is provided for you.
  return viterbi_backtrace_word_tree(graph, curFrame, wordTree, outLabelList);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Routine for copying data from FrameData object into chart.
 *
 *   For returning debugging info, if desired.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void copy_frame_to_chart(const FrameData& curFrame, unsigned frmIdx,
                         matrix<FrameCell>& chart) {
  if ((frmIdx >= chart.size1()) ||
      (curFrame.get_state_count() != chart.size2()))
    throw runtime_error("Mismatch in chart size.");
  unsigned activeCnt = curFrame.size();
  for (unsigned activeIdx = 0; activeIdx < activeCnt; ++activeIdx) {
    unsigned stateIdx = curFrame.get_state_by_index(activeIdx);
    chart(frmIdx, stateIdx) = curFrame.get_cell_by_state(stateIdx);
  }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Routine for Viterbi backtrace; token passing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double viterbi_backtrace_word_tree(const Graph& graph,
                                   const FrameData& curFrame,
                                   const WordTree& wordTree,
                                   vector<int>& outLabelList) {
  //  Find best final state.
  vector<int> finalStates;
  int finalCnt = graph.get_final_state_list(finalStates);
  double bestLogProb = g_zeroLogProb;
  int bestFinalState = -1;
  for (int finalIdx = 0; finalIdx < finalCnt; ++finalIdx) {
    int stateIdx = finalStates[finalIdx];
    if (!curFrame.has_cell(stateIdx)) continue;
    const FrameCell& curCell = curFrame.get_cell_by_state(stateIdx);
    double curLogProb =
        curCell.get_log_prob() + graph.get_final_log_prob(stateIdx);
    if (curLogProb > bestLogProb)
      bestLogProb = curLogProb, bestFinalState = stateIdx;
  }
  if (bestFinalState < 0) throw runtime_error("No complete paths found.");

  //  Do back trace to recover best word sequence.
  outLabelList.clear();
  for (int curNode =
           curFrame.get_cell_by_state(bestFinalState).get_node_index();
       curNode != (int)wordTree.get_root_node();
       curNode = wordTree.get_parent_node(curNode))
    outLabelList.push_back(wordTree.get_last_word(curNode));
  reverse(outLabelList.begin(), outLabelList.end());
  return bestLogProb;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static double get_cur_time() {
  static int ticksPerSec;
  if (!ticksPerSec) ticksPerSec = sysconf(_SC_CLK_TCK);

  struct tms tmsBuf;
  times(&tmsBuf);
  return (double)(tmsBuf.tms_utime + tmsBuf.tms_stime) / ticksPerSec;
}

void Timer::start() {
  if (is_on()) throw runtime_error("Timer already on.");
  m_start = get_cur_time();
}

double Timer::stop() {
  if (!is_on()) throw runtime_error("Timer currently off.");
  m_cumSecs += get_cur_time() - m_start;
  m_start = -1.0;
  return m_cumSecs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Sorry we don't have attila lib.
// extern GmmScorer* new_gmm_set_attila(const string& fileBase);

static GmmScorer* new_gmm_scorer(const map<string, string>& params) {
  string gmmFile = get_string_param(params, "gmm");
  string attilaGmmFile = get_string_param(params, "attila_gmm");
  if (gmmFile.empty() == attilaGmmFile.empty())
    throw runtime_error(
        "Must supply exactly one of 'gmm' or "
        "'attila_gmm' parameters.");
  if (!gmmFile.empty())
    return new GmmSet(gmmFile);
  else
    throw runtime_error(
        "Do not support use attila_gmm!");
  // return new_gmm_set_attila(attilaGmmFile);
}

Lab4VitMain::Lab4VitMain(const map<string, string>& params)
    : m_params(params),
      m_frontEnd(m_params),
      m_gmmScorerPtr(new_gmm_scorer(m_params)),
      m_audioStrm(get_required_string_param(m_params, "audio_file").c_str()),
      m_graph(get_required_string_param(m_params, "graph_file"),
              get_required_string_param(m_params, "word_syms")),
      m_outStrm(get_required_string_param(m_params, "dcd_file").c_str()),
      m_acousWgt(get_float_param(m_params, "ac_wgt", 0.1)),
      m_logProbBeam(get_float_param(m_params, "log10_beam", 5) * log(10.0)),
      m_stateCntBeam(get_int_param(m_params, "rank_beam", 1000)),
      m_totFrmCnt(0),
      m_totLogProb(0.0) {
  if (m_graph.get_gmm_count() > m_gmmScorerPtr->get_gmm_count())
    throw runtime_error(
        "Mismatch in number of GMM's between "
        "FSM and GmmSet.");
  if (m_graph.get_start_state() < 0)
    throw runtime_error("Graph has no start state.");
  if ((m_stateCntBeam > 0) && (m_logProbBeam > 10000.0))
    throw runtime_error(
        "Can't have beam > 10000 (base e) "
        "with state pruning.");
}

bool Lab4VitMain::init_utt() {
  if (m_audioStrm.peek() == EOF) return false;

  m_idStr = read_float_matrix(m_audioStrm, m_inAudio);
  cout << "Processing utterance ID: " << m_idStr << endl;
  m_frontEndTimer.start();
  m_frontEnd.get_feats(m_inAudio, m_feats);
  m_frontEndTimer.stop();
  if (m_feats.size2() != m_gmmScorerPtr->get_dim_count())
    throw runtime_error("Mismatch in GMM and feat dim.");
  m_gmmTimer.start();
  m_gmmScorerPtr->calc_gmm_probs(m_feats, m_gmmProbs);
  m_gmmTimer.stop();
  if (!get_string_param(m_params, "chart_file").empty()) {
    m_chart.resize(m_feats.size1() + 1, m_graph.get_state_count());
    m_chart.clear();
  }
  m_searchTimer.start();
  return true;
}

void Lab4VitMain::finish_utt(double logProb) {
  m_searchTimer.stop();
  m_totFrmCnt += m_feats.size1();
  m_totLogProb += logProb;

  //  Output results.
  cout << "  Output:";
  for (int labelIdx = 0; labelIdx < (int)m_labelList.size(); ++labelIdx) {
    m_outStrm << m_graph.get_word_sym_table().get_str(m_labelList[labelIdx])
              << " ";
    cout << " " << m_graph.get_word_sym_table().get_str(m_labelList[labelIdx]);
  }
  m_outStrm << "(" << m_idStr << ")" << endl;
  cout << endl;

  string chartFile = get_string_param(m_params, "chart_file");
  if (!chartFile.empty()) {
    //  Output DP chart, for debugging.
    ofstream chartStrm(chartFile.c_str());
    int frmCnt = m_feats.size1();
    int stateCnt = m_graph.get_state_count();
    matrix<double> matProbs(frmCnt + 1, stateCnt);
    matrix<int> matTrace(frmCnt + 1, stateCnt);
    vector<int> vecActive(frmCnt + 1, 0);
    for (int frmIdx = 0; frmIdx <= frmCnt; ++frmIdx) {
      for (int srcIdx = 0; srcIdx < stateCnt; ++srcIdx) {
        matProbs(frmIdx, srcIdx) = m_chart(frmIdx, srcIdx).get_log_prob();
        matTrace(frmIdx, srcIdx) = m_chart(frmIdx, srcIdx).get_node_index();
        if (matProbs(frmIdx, srcIdx) != g_zeroLogProb) ++vecActive[frmIdx];
      }
    }
    write_float_matrix(chartStrm, matProbs, m_idStr + "_probs");
    write_int_matrix(chartStrm, matTrace, m_idStr + "_trace");
    write_int_vector(chartStrm, vecActive, m_idStr + "_active");
    chartStrm.close();
  }
}

void Lab4VitMain::finish() {
  m_audioStrm.close();
  m_outStrm.close();
  cout << format("%.6f logprob/frame (%d frames).") %
              (m_totFrmCnt ? m_totLogProb / m_totFrmCnt : 0.0) % m_totFrmCnt
       << endl;
  double totSecs = m_frontEndTimer.get_cum_secs() + m_gmmTimer.get_cum_secs() +
                   m_searchTimer.get_cum_secs() + 1e-3;
  double framesPerSec =
      get_float_param(m_params, "window.frames_per_sec", 100.0);
  cout << format("%.2f xRT (%.1f%% front end; %.1f%% GMM; %.1f%% search)") %
              (totSecs / ((m_totFrmCnt + 1e-3) / framesPerSec)) %
              (100.0 * m_frontEndTimer.get_cum_secs() / totSecs) %
              (100.0 * m_gmmTimer.get_cum_secs() / totSecs) %
              (100.0 * m_searchTimer.get_cum_secs() / totSecs)
       << endl;
}

#ifndef NO_MAIN_LOOP

void main_loop(const char** argv) {
  map<string, string> params;
  process_cmd_line(argv, params);

  Lab4VitMain mainObj(params);
  while (mainObj.init_utt()) {
    double logProb = do_viterbi(
        mainObj.get_graph(), mainObj.get_gmm_probs(), mainObj.get_label_list(),
        mainObj.get_acous_wgt(), mainObj.get_log_prob_beam(),
        mainObj.get_state_count_beam(), mainObj.get_chart());
    mainObj.finish_utt(logProb);
  }
  mainObj.finish();
}

#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
