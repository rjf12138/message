#include "msg_record.h"

namespace my_utils {
//////////////////////////////////////////////
// 默认标准输出函数
int output_to_stdout(const string &msg)
{
    std::cout << msg << std::endl;

    return 0;
}

// 默认标准出错函数
int output_to_stderr(const string &msg)
{
    std::cerr << msg << std::endl;

    return 0;
}
//////////////////////////////////////////////

MsgRecord::MsgRecord(void)
: msg_to_stream_trace_(output_to_stdout),
  msg_to_stream_debug_(output_to_stdout),
  msg_to_stream_info_(output_to_stdout),
  msg_to_stream_warn_(output_to_stderr),
  msg_to_stream_fatal_(output_to_stderr)
{

}

MsgRecord::~MsgRecord(void)
{
}

void 
MsgRecord::set_stream_func(InfoLevel level, msg_to_stream_callback func)
{
    switch (level) {
        case LOG_LEVEL_TRACE:
            msg_to_stream_trace_ = func; break;
        case LOG_LEVEL_DEBUG:
            msg_to_stream_debug_ = func; break;
        case LOG_LEVEL_INFO:
            msg_to_stream_info_ = func; break;
        case LOG_LEVEL_WARN:
            msg_to_stream_warn_ = func; break;
        case LOG_LEVEL_ERROR:
            msg_to_stream_error_ = func; break;
        case LOG_LEVEL_FATAL:
            msg_to_stream_fatal_ = func; break;
        default: 
        {
            output_to_stderr("MsgRecord::set_stream_func: Unknown option!");
        } break;
    }
}

msg_to_stream_callback 
MsgRecord::get_stream_func(InfoLevel level)
{
    switch (level) {
        case LOG_LEVEL_TRACE:
            return msg_to_stream_trace_;
        case LOG_LEVEL_DEBUG:
            return msg_to_stream_debug_ ;
        case LOG_LEVEL_INFO:
            return msg_to_stream_info_;
        case LOG_LEVEL_WARN:
            return msg_to_stream_warn_;
        case LOG_LEVEL_ERROR:
            return msg_to_stream_error_;
        case LOG_LEVEL_FATAL:
            return msg_to_stream_fatal_;
        default: 
        {
            output_to_stderr("MsgRecord::set_stream_func: Unknown option!");
        } break;
    }

    return nullptr;
}

void
MsgRecord::print_msg(InfoLevel level, int line, string file_name, const char *format, ...)
{
    uint32_t msg_len = strlen(format)+1;
    char *msg_buff = new char[msg_len];
    memset(msg_buff, 0, msg_len);

    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsprintf(msg_buff, format, arg_ptr);
    va_end(arg_ptr);

    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));

    MsgContent tmp_msg;
    tmp_msg.info_level = level;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());
    msg_info_.push_back(tmp_msg);
    delete msg_buff;
    // 组装缓存中的所有消息
    string print_msg = this->assemble_msg();
    // 向流中输出消息
    this->get_stream_func(level)(print_msg);

    return ;
}

string 
MsgRecord::get_msg_attr(InfoLevel level, int line, string file_name, const char *format, ...)
{
    char *msg_buff = new char[1024];
    memset(msg_buff, 0, 1024);

    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsnprintf(msg_buff, 1024,format, arg_ptr);
    va_end(arg_ptr);

    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));

    MsgContent tmp_msg;
    tmp_msg.info_level = level;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());
    msg_info_.push_back(tmp_msg);

    delete msg_buff;
    msg_buff = nullptr;
    
    // 组装缓存中的所有消息
    return this->assemble_msg();
}

string
MsgRecord::assemble_msg(void)
{
    ostringstream ostr;
    for (std::size_t i = 0; i < msg_info_.size(); ++i) {
        ostr << "[" << msg_info_[i].when << "]";
        ostr << "[" << msg_info_[i].which_line << "]";
        ostr << "[" << msg_info_[i].which_file << "] ";
        ostr << "[" << level_convert(msg_info_[i].info_level) << "]  ";
        ostr << msg_info_[i].msg_info << std::endl;
    }
    msg_info_.clear();
    return ostr.str();
}

string 
MsgRecord::level_convert(enum InfoLevel level)
{
    switch(level)
    {
        case LOG_LEVEL_TRACE:
            return "TRACE";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_FATAL:
            return "FATAL";
    }

    return "UNKNOWN_LOG_LEVEL";
}

}