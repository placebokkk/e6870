
//  $Id: lab4_vit.H,v 1.7 2009/11/05 14:25:21 stanchen Exp stanchen $

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   @file lab4_vit.H
 *   @brief Main loop for Lab 4 Viterbi decoder.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _LAB4_VIT_H
#define _LAB4_VIT_H

#include <functional>
#include <utility>
#include "front_end.H"
#include "util.H"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   CPU timer.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Timer {
 public:
  /** Ctor; if @p doStart is true, starts timer. **/
  Timer(bool doStart = false) : m_cumSecs(0.0), m_start(-1.0) {
    if (doStart) start();
  }

  /** Returns whether timer is currently on. **/
  bool is_on() const { return m_start != -1.0; }

  /** Starts timer. **/
  void start();

  /** Stops timer.  Returns cumulative time on so far. **/
  double stop();

  /** Returns cumulative seconds timer has been on so far.
   *   If timer currently on, doesn't include time since last
   *   time was started.
   **/
  double get_cum_secs() const { return m_cumSecs; }

 private:
  /** Cumulative seconds timer has been on so far. **/
  double m_cumSecs;

  /** If timer on, last time timer was started; -1 otherwise. **/
  double m_start;
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Struct for holding a backtrace word tree.
 *
 *   This object can be used to hold a list of word sequences in
 *   the form of a tree.  Each node in the tree is assigned an
 *   integer index, and each arc in the tree is labeled with an
 *   integer index corresponding to a word label.  Each node in
 *   the tree can be viewed as representing the word sequence
 *   labeling the path from the root to that node.
 *
 *   To get the index of the root node, use #get_root_node().
 *   To find/create the node reached by extending a node with a word,
 *   use #insert_node().  To recover the word sequence a node
 *   corresponds to, you can use #get_parent_node() and #get_last_word().
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class WordTree {
 private:
  typedef pair<int, unsigned> Node;

 public:
  /** Ctor; initializes object to just contain root node. **/
  WordTree() { clear(); }

  /** Clears object except for root node. **/
  void clear() {
    m_nodeArray.clear();
    m_nodeHash.clear();
    insert_node((unsigned)-1, (unsigned)-1);
  }

  /** Returns number of nodes in tree. **/
  unsigned size() const { return m_nodeArray.size(); }

  /** Returns index of root node. **/
  unsigned get_root_node() const { return 0; }

  /** Given an existing node @p parentIdx, returns index of
   *   child node reached when traversing arc labeled with
   *   word index @p lastWord.  If node doesn't exist, it is created.
   **/
  unsigned insert_node(unsigned parentIdx, unsigned lastWord) {
    Node key(parentIdx, lastWord);
    map<Node, unsigned>::const_iterator itemPtr = m_nodeHash.find(key);
    if (itemPtr != m_nodeHash.end()) return itemPtr->second;

    m_nodeArray.push_back(Node(parentIdx, lastWord));
    unsigned nodeIdx = m_nodeArray.size() - 1;
    m_nodeHash[key] = nodeIdx;
    return nodeIdx;
  }

  /** Returns index of parent node for node with index @p nodeIdx. **/
  unsigned get_parent_node(unsigned nodeIdx) const {
    return m_nodeArray[nodeIdx].first;
  }

  /** Returns index of word labeling arc from node @p nodeIdx
   *   to its parent node.
   **/
  unsigned get_last_word(unsigned nodeIdx) const {
    return m_nodeArray[nodeIdx].second;
  }

 private:
  /** Array of nodes in tree. */
  vector<Node> m_nodeArray;

  /** Hash table, for fast node lookup. */
  map<Node, unsigned> m_nodeHash;
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Cell in dynamic programming chart for Viterbi algorithm.
 *
 *   Holds Viterbi log prob; and arc ID of best incoming arc for backtrace.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class FrameCell {
 public:
  /** Ctor; inits log prob to g_zeroLogProb and node index to 0. **/
  FrameCell() : m_logProb(g_zeroLogProb), m_nodeIdx(0) {}

#ifndef SWIG
#ifndef DOXYGEN
  //  Hack; for bug in matrix<> class in boost 1.32.
  explicit FrameCell(int) : m_logProb(g_zeroLogProb), m_nodeIdx(0) {}
#endif
#endif

  /** Sets associated log prob and WordTree node index. **/
  void assign(double logProb, unsigned nodeIdx) {
    m_logProb = logProb;
    m_nodeIdx = nodeIdx;
  }

  /** Returns log prob of cell. **/
  double get_log_prob() const { return m_logProb; }

  /** Returns node index in WordTree for best incoming word sequence. **/
  unsigned get_node_index() const { return m_nodeIdx; }

 private:
  /** Forward Viterbi logprob. **/
  float m_logProb;

  /** Holds node index in WordTree for best incoming word sequence. **/
  unsigned m_nodeIdx;
};

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Struct for holding (active) cells at a frame in the DP chart.
 *
 *   Stores a list of cells of type FrameCell.
 *
 *   To find a cell and create it if it doesn't exist, use #insert_cell().
 *   To look up cells by state index, use #get_cell_by_state() and #has_cell().
 *
 *   To loop through all cells in increasing state order, use
 *   #reset_iteration() and #get_next_state().
 *
 *   To loop through all cells (in no particular order), use
 *   #get_cell_by_index() (and #size() to determine how many cells there are).
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class FrameData {
 public:
  /** Ctor; initializes object to be empty.
   *   The argument @p stateCnt should be the number of states
   *   in the graph.
   **/
  FrameData(unsigned stateCnt) : m_stateMap(stateCnt, -1), m_heapSize(-1) {}

  /** Clears object. **/
  void clear() {
    vector<unsigned>::const_iterator endPtr = m_activeStates.end();
    for (vector<unsigned>::const_iterator curPtr = m_activeStates.begin();
         curPtr != endPtr; ++curPtr) {
      assert(m_stateMap[*curPtr] >= 0);
      m_stateMap[*curPtr] = -1;
    }
    m_activeStates.clear();
    m_cellArray.clear();
    m_heapSize = -1;
  }

  /** Returns number of active cells. **/
  unsigned size() const { return m_cellArray.size(); }

  /** Returns whether no active cells. **/
  bool empty() const { return m_cellArray.empty(); }

  /** Returns number of states in corresponding graph. **/
  unsigned get_state_count() const { return m_stateMap.size(); }

  /** Returns cell corresponding to state with index @p stateIdx.
   *   The cell must already exist.  You can check whether a cell
   *   exists using #has_cell().
   **/
  const FrameCell& get_cell_by_state(unsigned stateIdx) const {
    assert(m_stateMap[stateIdx] >= 0);
    return m_cellArray[m_stateMap[stateIdx]];
  }

  /** Returns whether cell exists for state @p stateIdx. **/
  bool has_cell(unsigned stateIdx) const { return m_stateMap[stateIdx] >= 0; }

  /** Returns cell for state @p stateIdx, creating it if absent.
   *   If called in the middle of looping through states
   *   (see #reset_iteration(), #get_next_state()),
   *   will be added into list of states not yet looped through.
   **/
  FrameCell& insert_cell(unsigned stateIdx) {
    int cellIdx = m_stateMap[stateIdx];
    if (cellIdx >= 0) return m_cellArray[cellIdx];
    m_activeStates.push_back(stateIdx);
    if (m_heapSize >= 0) {
      std::swap(m_activeStates.back(), m_activeStates[m_heapSize]);
      ++m_heapSize;
      push_heap(m_activeStates.begin(), m_activeStates.begin() + m_heapSize,
                greater<unsigned>());
    }
    m_cellArray.push_back(FrameCell());
    m_stateMap[stateIdx] = m_cellArray.size() - 1;
    return m_cellArray.back();
  }

  /** Returns cell with index @p cellIdx, where cells are numbered
   *   in an arbitrary order.
   *   Cells are numbered upwards from 0.  There is no easy way to
   *   recover the state index corresponding to a cell retrieved
   *   by this method.  However, this method may be useful for
   *   computing pruning thresholds.
   **/
  const FrameCell& get_cell_by_index(unsigned cellIdx) const {
    return m_cellArray[cellIdx];
  }

  /** Returns state index for @p idx-th active state, where states
   *   are numbered in no particular order.
   *   If any non-read-only methods are called, the numbering
   *   of states may change.
   **/
  unsigned get_state_by_index(unsigned idx) const {
    return m_activeStates[idx];
  }

  /** Prepares object for iterating through states in upward order.
   *   See #get_next_state() to do actual iteration.  Specifically,
   *   puts all active states in list of states not yet iterated through.
   **/
  void reset_iteration() {
    make_heap(m_activeStates.begin(), m_activeStates.end(),
              greater<unsigned>());
    m_heapSize = m_activeStates.size();
  }

  /** Returns lowest-numbered state not yet iterated through,
   *   or -1 if no more active states.
   **/
  int get_next_state() {
    assert(m_heapSize >= 0);
    if (!m_heapSize) return -1;
    pop_heap(m_activeStates.begin(), m_activeStates.begin() + m_heapSize,
             greater<unsigned>());
    --m_heapSize;
    return m_activeStates[m_heapSize];
  }

  /** Swap operation. **/
  void swap(FrameData& frmData) {
    m_activeStates.swap(frmData.m_activeStates);
    m_cellArray.swap(frmData.m_cellArray);
    m_stateMap.swap(frmData.m_stateMap);
    std::swap(m_heapSize, frmData.m_heapSize);
  }

 private:
  /** The states that are active, in no particular order. **/
  vector<unsigned> m_activeStates;

  /** Array of DP cells for active states. **/
  vector<FrameCell> m_cellArray;

  /** For each state, location in m_cellArray if active, -1 otherwise.
   *   That is, for an inactive state @c stateIdx, @c m_stateMap[stateIdx]
   *   will be -1.  For an active state @c stateIdx, its DP cell can be
   *   found at @c m_cellArray[m_stateMap[stateIdx]].
   **/
  vector<int> m_stateMap;

  /** If nonnegative, how many states in heap in m_activeStates.
   *   That is, we loop through states in a frame in order by
   *   keeping the states in a "heap".  A "heap" is formed
   *   by ordering the elements in an array in such a way that
   *   it is easy to keep track of the lowest-numbered element.
   *   Before we begin looping through states in order,
   *   we arrange all states in m_activeStates as a heap.
   *   We then repeatedly grab the lowest-numbered state in the heap,
   *   and then remove the state from the heap.
   *   The array m_activeStates stays the same size
   *   during this, but the part of m_activeStates
   *   arranged as a heap becomes smaller and smaller.
   **/
  int m_heapSize;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** Routine for copying debugging info from @p curFrame into @p chart. **/
void copy_frame_to_chart(const FrameData& curFrame, unsigned frmIdx,
                         matrix<FrameCell>& chart);

/** Routine for Viterbi backtrace; token passing. **/
double viterbi_backtrace_word_tree(const Graph& graph,
                                   const FrameData& lastFrame,
                                   const WordTree& wordTree,
                                   vector<int>& outLabelList);

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *   Encapsulation of main loop for Viterbi decoding.
 *
 *   Holds global variables and has routines for initializing variables
 *   and updating them for each utterance.
 *   We do this so that we can call this code from Java as well.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class Lab4VitMain {
 public:
  /** Initialize all data given parameters. **/
  Lab4VitMain(const map<string, string>& params);

  /** Called at the beginning of processing each utterance.
   *   Returns whether at EOF.
   **/
  bool init_utt();

  /** Called at the end of processing each utterance. **/
  void finish_utt(double logProb);

  /** Called at end of program. **/
  void finish();

  /** Returns decoding graph/HMM. **/
  const Graph& get_graph() const { return m_graph; }

  /** Returns matrix of GMM log probs for each frame. **/
  const matrix<double>& get_gmm_probs() const { return m_gmmProbs; }

  /** Returns vector to place decoded labels in. **/
  vector<int>& get_label_list() { return m_labelList; }

  /** Returns acoustic weight. **/
  double get_acous_wgt() const { return m_acousWgt; }

  /** Returns beam width, log base e. **/
  double get_log_prob_beam() const { return m_logProbBeam; }

  /** Returns rank beam; 0 signals no rank pruning. **/
  unsigned get_state_count_beam() const { return m_stateCntBeam; }

  /** Returns full DP chart; only used for storing diagnostic info. **/
  matrix<FrameCell>& get_chart() { return m_chart; }

 private:
  /** Program parameters. **/
  map<string, string> m_params;

  /** Front end. **/
  FrontEnd m_frontEnd;

  /** Acoustic model. **/
  shared_ptr<GmmScorer> m_gmmScorerPtr;

  /** Stream for reading audio data. **/
  ifstream m_audioStrm;

  /** Graph/HMM. **/
  Graph m_graph;

  /** Stream for writing decoding output to. **/
  ofstream m_outStrm;

  /** Acoustic weight. **/
  double m_acousWgt;

  /** Beam width, log base e. **/
  double m_logProbBeam;

  /** Rank beam; 0 signals no rank pruning. **/
  unsigned m_stateCntBeam;

  /** ID string for current utterance. **/
  string m_idStr;

  /** Input audio for current utterance. **/
  matrix<double> m_inAudio;

  /** Feature vectors for current utterance. **/
  matrix<double> m_feats;

  /** GMM probs for current utterance. **/
  matrix<double> m_gmmProbs;

  /** Decoded output. **/
  vector<int> m_labelList;

  /** DP chart for current utterance, for returning diagnostic info. **/
  matrix<FrameCell> m_chart;

  /** Total frames processed so far. **/
  int m_totFrmCnt;

  /** Total log prob of utterances processed so far. **/
  double m_totLogProb;

  /** Timer for front end processing. **/
  Timer m_frontEndTimer;

  /** Timer for GMM prob computation. **/
  Timer m_gmmTimer;

  /** Timer for search computation. **/
  Timer m_searchTimer;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif
