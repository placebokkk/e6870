
//  $Id: lab1_frontend.C,v 1.6 2009/09/17 14:47:08 stanchen Exp $

#include "front_end.H"
#include "util.H"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void MainLoop(const char** argv) {
  //  Process command line arguments.
  map<string, string> params;
  process_cmd_line(argv, params);

  //  Initialize front end.
  FrontEnd front_end(params);

  //  Main loop.
  ifstream audio_strm(get_required_string_param(params, "audio_file").c_str());
  ofstream feat_strm(get_required_string_param(params, "feat_file").c_str());
  matrix<double> in_audio, feats;
  while (audio_strm.peek() != EOF) {
    string id_str = read_float_matrix(audio_strm, in_audio);
    cout << "Processing utterance ID: " << id_str << endl;
    front_end.get_feats(in_audio, feats);
    write_float_matrix(feat_strm, feats, id_str);
  }
  audio_strm.close();
  feat_strm.close();
}

int main(int argc, const char** argv) {
  //  Catch all exceptions; print error message if exception caught.
  try {
    MainLoop(argv);
  } catch (exception& xc) {
    cerr << "Error: " << xc.what() << endl;
    return -1;
  }
  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
