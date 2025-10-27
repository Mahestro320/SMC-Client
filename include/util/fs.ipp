
template<typename StreamType>
void util::fs::freeStream(StreamType*& stream) {
    if (stream) {
        stream->close();
        delete stream;
        stream = nullptr;
    }
}
