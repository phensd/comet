#include "include/mpris_signal_handler.h"
#include "include/mpris_function.h"

void comet::mpris_signal_handler::push_signal(comet::mpris_signal_handler::signal signal){
    const std::lock_guard<std::mutex> lock(signals_mutex);
    signals.push(signal);
}


std::function<void (comet::player* player)> comet::mpris_signal_handler::get_function_from_signal(comet::mpris_signal_handler::signal signal){
    using sig = comet::mpris_signal_handler::signal;
    switch(signal){
        case sig::PLAY:
            return comet::mpris_function::mpris_function_play;
            break;
        case sig::PLAY_NEXT:
            return comet::mpris_function::mpris_function_play_next;
            break;
        case sig::PREVIOUS:
            return comet::mpris_function::mpris_function_previous;
            break;
        case sig::PLAY_PAUSE:
            return comet::mpris_function::mpris_function_play_or_pause;
          break;
        }
}


std::optional<std::function<void (comet::player* player)>> comet::mpris_signal_handler::pulse(){

    const std::lock_guard<std::mutex> lock(signals_mutex);

    //if there are no signals then do nothing.
    if(signals.size() < 1) return std::nullopt;

    //otherwise send over the function matching the signal and pop it.
    auto top_signal {signals.front()};
    signals.pop();
    return get_function_from_signal(top_signal);
}
