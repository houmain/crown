"use strict";var WA=WA||{};!function(){var r=WA.print||(WA.print=r=>console.log(r.replace(/\n$/,""))),e=WA.error||(WA.error=(e,t)=>r("[ERROR] "+e+": "+t+"\n")),WM,ASM,t,MU8,MU16,MU32,MI32,MF32,a,n=WA.maxmem||268435456,STOP,abort=WA.abort=(r,t)=>{throw STOP=!0,e(r,t),"abort"},MStrPut=(r,e,t)=>{if(0===t)return 0;var a=(new TextEncoder).encode(r),n=a.length,o=e||ASM.malloc(n+1);if(t&&n>=t)for(n=t-1;128==(192&a[n]);n--);return MU8.set(a.subarray(0,n),o),MU8[o+n]=0,e?n:o},MStrGet=(r,e)=>{if(0===e||!r)return"";if(!e)for(e=0;e!=r+MU8.length&&MU8[r+e];e++);return(new TextDecoder).decode(MU8.subarray(r,r+e))},MArrPut=r=>{var e=r.byteLength||r.length,t=e&&ASM.malloc(e);return MU8.set(r,t),t},o=()=>{var r=t.buffer;MU8=new Uint8Array(r),MU16=new Uint16Array(r),MU32=new Uint32Array(r),MI32=new Int32Array(r),MF32=new Float32Array(r)},i=WA.module;i||(i="o"==(typeof process)[0]?require("fs").readFileSync(process.argv[2]):document.currentScript.getAttribute("data-wasm")),("s"==(typeof i)[0]?fetch(i).then(r=>r.arrayBuffer()):new Promise(r=>r(i))).then(e=>WebAssembly.compile(e).then(i=>{var s=()=>0,l=r=>abort("CRASH",r),J={},c={sbrk:r=>{var e=a,i=e+r,s=i-t.buffer.byteLength;return i>n&&abort("MEM","Out of memory"),s>0&&(t.grow(s+65535>>16),o()),a=i,e},time:r=>{var e=Date.now()/1e3|0;return r&&(MU32[r>>2]=e),e},gettimeofday:r=>{var e=Date.now();MU32[r>>2]=e/1e3|0,MU32[r+4>>2]=e%1e3*1e3|0},__assert_fail:(r,e,t,a)=>l("assert "+MStrGet(r)+" at: "+(e?MStrGet(e):"?"),t,a?MStrGet(a):"?")},f={env:c,J:J},m={},N={};for(var u in WebAssembly.Module.imports(i).forEach(a=>{var n=a.module,o=a.name,i=a.kind[0],u=f[n]||(f[n]={});if("m"==i)for(let r,a,n,i,s,l=new Uint8Array(e),c=8,f=l.length;c<f&&(s=r=>{c+=0|r;for(var e,t,a=0;t|=(127&(e=l[c++]))<<a,e>>7;a+=7);return t},a=s(),n=s(),r=c+n,!(a<0||a>11||n<=0||r>f));c=r)if(2==a)for(n=s(),i=0;i!=n&&c<r;i++,1==a&&s(1)&&s(),2>a&&s(),3==a&&s(1))2==(a=s(s(s())))&&(t=u[o]=new WebAssembly.Memory({initial:s(1)}),c=r=f);if("f"==i){if(u==J){let[r,e,t,a,n]=o.split("");if(!t&&!n)return;a||(a=""),m[a]||(m[a]=""),e=e.replace(/^\(\s*void\s*\)$|^\(|\[.*?\]|(=|WA_ARG\()[^,]+|\)$/g,"").replace(/.*?(\w+)\s*(,|$)/g,"$1$2"),m[a]+=(n||"").replace(/^\(?\s*|\s*\)$/g,"")+"J[N."+r+"]=("+e+")=>"+t+";",N[r]=o}u!=c||c[o]||(u[o]=Math[o.replace(/^f?([^l].*?)f?$/,"$1").replace(/^rint$/,"round")]||o.match(/uncaught_excep|pure_virt|^abort$|^longjmp$/)&&(()=>l(o))||s,c[o]==s&&console.log("[WASM] Importing empty function for env."+o)),n.includes("wasi")&&(u[o]=o.includes("write")?(e,t,a,n)=>{t>>=2;for(var o=0,i="",s=0;s<a;s++){var l=MU32[t++],c=MI32[t++];if(c<0)return-1;o+=c,i+=MStrGet(l,c)}return r(i),MU32[n>>2]=o,0}:s)}}),m)try{(()=>{eval(m[u].replace(/[\0-\37]/g,r=>"\\x"+escape(r).slice(1)))})()}catch(r){abort("BOOT","Error in #WAJIC function: "+r+"("+m[u]+")")}return WA.wm=WM=i,WebAssembly.instantiate(i,f)})).then(r=>{WA.asm=ASM=r.exports;var e=ASM.memory,n=ASM.__wasm_call_ctors,i=ASM.main||ASM.__main_argc_argv,s=ASM.__original_main||ASM.__main_void,l=ASM.malloc,c=ASM.WajicMain,f=WA.started;if(e&&(t=e),t&&(o(),a=MU8.length),n&&n(),i&&l){var m=l(10);MU8[m+8]=87,MU8[m+9]=0,MU32[m>>2]=m+8,MU32[m+4>>2]=0,i(1,m)}else i&&i(0,0);s&&s(),c&&c(),f&&f()}).catch(r=>{"abort"!==r&&WA.error("BOOT","WASM instiantate error: "+r+(r.stack?"\n"+r.stack:""))})}();