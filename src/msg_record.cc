#include "msg_record.h"

namespace my_utils {
//////////////////////////////////////////////
// 默认标准输出函数
int output_to_stdout(const string &msg, void *arg)
{
    std::cout << msg << std::endl;

    return 0;
}

// 默认标准出错函数
int output_to_stderr(const string &msg, void *arg)
{
    std::cerr << msg << std::endl;

    return 0;
}


MsgRecord MsgRecord::g_log_msg;

MsgRecord::MsgRecord(void)
: msg_to_stream_trace_(output_to_stdout),
  msg_to_stream_debug_(output_to_stdout),
  msg_to_stream_info_(output_to_stdout),
  msg_to_stream_warn_(output_to_stderr),
  msg_to_stream_error_(output_to_stderr),
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
MsgRecord::print_msg(InfoLevel level, int line, string file_name, string func, const char *format, ...)
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
    tmp_msg.msg_func = func;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());

    ostringstream ostr;
    ostr << "[" << tmp_msg.when << "]";
    ostr << "[" << tmp_msg.msg_func << "]";
    ostr << "[" << tmp_msg.which_file << ":" << tmp_msg.which_line << "]";
    ostr << "[" << level_convert(tmp_msg.info_level) << "]: ";
    ostr << tmp_msg.msg_info;

    delete[] msg_buff;
    msg_buff = nullptr;

    this->get_stream_func(level)(ostr.str());
    
    return ;
}

string 
MsgRecord::get_msg_attr(InfoLevel level, int line, string file_name, string func, const char *format, ...)
{
    char *msg_buff = new char[2048];
    memset(msg_buff, 0, 2048);

    va_list arg_ptr;
    va_start(arg_ptr,format);
    vsnprintf(msg_buff, 2048,format, arg_ptr);
    va_end(arg_ptr);

    char strtime[65] = {0};
    time_t curr_time = time(NULL);
    strftime(strtime, sizeof(strtime), "%Y-%m-%d %H:%M:%S", localtime(&curr_time));

    MsgContent tmp_msg;
    tmp_msg.info_level = level;
    tmp_msg.when = strtime;
    tmp_msg.msg_info = msg_buff;
    tmp_msg.msg_func = func;
    tmp_msg.which_line = line;
    tmp_msg.which_file = basename(file_name.c_str());

    ostringstream ostr;
    ostr << "[" << tmp_msg.when << "]";
    ostr << "[" << tmp_msg.msg_func << "]";
    ostr << "[" << tmp_msg.which_file << ":" << tmp_msg.which_line << "]";
    ostr << "[" << level_convert(tmp_msg.info_level) << "]: ";
    ostr << tmp_msg.msg_info;

    delete[] msg_buff;
    msg_buff = nullptr;

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