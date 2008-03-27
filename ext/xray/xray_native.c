#include "ruby.h"
#include "node.h"
#include "env.h"
#include "ruby.h"

static VALUE frame_backtrace(struct FRAME *frame, int lev)
{
    char buf[BUFSIZ];
    VALUE ary;
    NODE *n;

    ary = rb_ary_new();
    if (NULL == frame) { return ary; }

    printf("Got frame %d level=%d\n", frame, lev);
    if (frame->last_func == ID_ALLOCATOR) {
        frame = frame->prev;
    }
    if (lev < 0) {
        printf("Level <0 \n");
        ruby_set_current_source();
        printf("Source set. last_func=%d\n", frame->last_func);
        if (frame->last_func) {
            snprintf(buf, BUFSIZ, "%s:%d:in `%s'",
               ruby_sourcefile, ruby_sourceline,
               rb_id2name(frame->last_func));
        } else if (ruby_sourceline == 0) {
            snprintf(buf, BUFSIZ, "%s", ruby_sourcefile);
        } else {
            snprintf(buf, BUFSIZ, "%s:%d", ruby_sourcefile, ruby_sourceline);
        }
        printf("updated buf with %s\n", buf);
        rb_ary_push(ary, rb_str_new2(buf));
        printf("Pushed in ary, lev=%d\n", lev);
        if (lev < -1) return ary;
    } else {
        while (lev-- > 0) {
            frame = frame->prev;
            if (!frame) {
                  ary = Qnil;
                  break;
            }
        }
   }

   printf("Entering for loop frame=%d\n", frame);
//   for (; frame && (n = frame->node); frame = frame->prev) {
//       if (frame->prev && frame->prev->last_func) {
 //          if (frame->prev->node == n) {
  //             if (frame->prev->last_func == frame->last_func) continue;
//           }
//           snprintf(buf, BUFSIZ, "%s:%d:in `%s'",
//               n->nd_file, nd_line(n),
//               rb_id2name(frame->prev->last_func));
//       } else {
//           snprintf(buf, BUFSIZ, "%s:%d", n->nd_file, nd_line(n));
//       }
//       rb_ary_push(ary, rb_str_new2(buf));
//    }

   return ary;
}

static void print_thread_info(rb_thread_t th)
{
  printf("======================================\n");
  printf("Thread id : %d\n", th);
  printf("    \\_ Status : %d\n", th->status);
  printf("    \\_ Priority : %d\n", th->priority);
  printf("    \\_ Node : %d\n", th->node);
  printf("    \\_ Frame : %d\n", th->frame);
  printf("    \\_ Next : %d\n", th->next);

}

static VALUE xray_backtrace(VALUE self)
{
  NODE *node;
  struct FRAME *frame;
  VALUE ary;
  rb_thread_t th;
  rb_thread_t th2;

  printf("XRay backtrace\n");
  Check_Type(self, T_DATA);
  th = (rb_thread_t) DATA_PTR(self);

  print_thread_info(th);
  ary = frame_backtrace(th2->frame, -1);

  return ary;  
}


VALUE rb_xray_dump_all_threads()
{
  NODE *node;
  struct FRAME *frame;
  rb_thread_t th;
  rb_thread_t th2;
  VALUE current_thread;
  
  current_thread = rb_thread_current();
  Check_Type(current_thread, T_DATA);
  th = (rb_thread_t) DATA_PTR(current_thread);

  print_thread_info(th);
  for (th2 = th->next; th2 && th2 != th; th2 = th2->next) {
    print_thread_info(th2);
  }
   
  return Qnil;  
}

void Init_xray_native() {
   rb_define_method(rb_cThread, "xray_backtrace", xray_backtrace, 0);
   rb_define_singleton_method(rb_cThread, "xray_dump_all_threads", rb_xray_dump_all_threads, 0);
}
